// Compile with g++ -std=c++11 -O4 dp.cc 

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <limits>
#include <math.h>
#include <cassert>
#include <unordered_map>
#include <cmath>
#include <iterator>

float MILLISECONDS_IN_SECOND = 1000;
float B_IN_MB = 1000000;
float M_IN_K = 1000;
float BITS_IN_BYTE = 8;
int RANDOM_SEED = 42;
float REBUF_PENALTY = 4.3;  // 1 sec rebuffering -> 3 Mbps
int SMOOTH_PENALTY = 1;
float INVALID_DOWNLOAD_TIME = -1;
float PACKET_PAYLOAD_PORTION = 0.95;
unsigned int LINK_RTT = 80;  // millisec
unsigned int PACKET_SIZE = 1500;  // bytes
float DT = 0.05;  // time granularity
static const int POSSIBLE_VIDEO_BIT_RATE[] = {150, 300, 450, 600, 1000, 1250, 1600, 2600, 3200, 3600, 4200, 6500};  // Kbps

// float VIDEO_CHUNCK_LEN = 4000;  // (ms), every time add this amount to buffer
// unsigned int BITRATE_LEVELS = 6;
// unsigned int TOTAL_VIDEO_CHUNCK = 49;
// float BUFFER_THRESH = 60;  // sec, max buffer limit

// unsigned int DEFAULT_QUALITY = 1;
// unsigned int MAX_QUALITY = 5;

std::string COOKED_TRACE_FOLDER = "./cooked_traces/";
std::string VIDEO_PARAMS_FOLDER = "./videos/";
std::string OUTPUT_FILE_PATH = "./results/log_sim_dp";
// std::string VIDEO_SIZE_FILE = "./video_size_";


struct ALL_COOKED_TIME_BW {
	std::vector<std::vector<float> > all_cooked_time;
	std::vector<std::vector<float> > all_cooked_bw;
	std::vector<std::string> all_file_names;
};

struct ONE_VIDEO_PARAMS {

	float VIDEO_CHUNCK_LEN;   // (ms), every time add this amount to buffer 4000
	unsigned int BITRATE_LEVELS ; // no of bitrate levels 6
	unsigned int TOTAL_VIDEO_CHUNCK ; // total chunks count 49
	float BUFFER_THRESH ;  // SECOND, max buffer limit 60
	int VIDEO_BIT_RATE[12]  = {-1};  // Kbps {300, 750, 1200, 1850, 2850, 4300}
	unsigned int DEFAULT_QUALITY; // zero-indexed default bitrate level 1
	unsigned int MAX_QUALITY; // 0-indexed maximum bitrate level 5
	std::vector<std::vector<unsigned int> > video_size;
		// [ [chunk1, chunk2, chunk3, chink4, .... {bitrate lowest}] , 
		//[chunk1, chunk2, chunk3, chink4, .... {bitrate secondlowest}] ,
		// [] ,
		// ......   ]	
	std::string file_name ; 	
};

struct DP_PT {
	unsigned int chunk_idx;
	unsigned int time_idx;
	unsigned int buffer_idx;
	unsigned int bit_rate;
};


uint64_t combine(uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
	// assume a, b, c, d < 2^16 = 65536
	assert(a < 65536);
	assert(b < 65536);
	assert(c < 65536);
	assert(d < 65536);
	return (a << 48) | (b << 32) | (c << 16) | d;
}


void split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
}


std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}


ALL_COOKED_TIME_BW get_all_cooked_time_bw(std::string path) {
	ALL_COOKED_TIME_BW all_cooked_time_bw;
	struct dirent *entry;
	DIR *dir = opendir(path.c_str());
	if (dir != NULL) {
		while ((entry = readdir(dir)) != NULL) {
			if (entry->d_name[0] == '.') continue;  // read . or ..
			// std::cout << "checkpoint 4" << '\n';
			std::string file_name = "";
			file_name += COOKED_TRACE_FOLDER;
			file_name += entry->d_name;
			printf("%s\n", file_name.c_str());

			std::ifstream in_file(file_name);
			std::string line;
			std::vector<float> cooked_time;
			std::vector<float> cooked_bw;

			if (in_file.is_open())
			{
				while ( getline (in_file, line) )
				{
					std::vector<std::string> parse ;
					std::istringstream iss(line);

					copy(std::istream_iterator <std::string> (iss), std::istream_iterator<std::string> () , std::back_inserter(parse) );


					cooked_time.push_back(std::stof(parse[0]));
					cooked_bw.push_back(std::stof(parse[1]));
				}
				in_file.close();
			}
			// std::cout << "checkpoint 3" << '\n';
			all_cooked_time_bw.all_cooked_time.push_back(cooked_time);
			all_cooked_time_bw.all_cooked_bw.push_back(cooked_bw);
			all_cooked_time_bw.all_file_names.push_back(entry->d_name);

		}
	}
	closedir(dir);

	return all_cooked_time_bw;
}

bool get_one_video_params(std::string file_name , ONE_VIDEO_PARAMS &cur_video) {
	/*
		New function : taken in the file name of video descriptor, parses the file 
		and populates ONE_VIDEO_PARAMS structure ; returns whether parsed or not 
	*/
	// std::vector< ONE_VIDEO_PARAMS > all_video_params;
	// struct dirent *entry;
	// DIR *dir = opendir(path.c_str());
	// if (dir != NULL) {
		// while ((entry = readdir(dir)) != NULL) {
			// if (entry->d_name[0] == '.') continue;  // read . or ..

			// std::string file_name = "";
			// file_name += VIDEO_PARAMS_FOLDER;
			// file_name += entry->d_name;
			// printf("%s\n", file_name.c_str());

			std::ifstream in_file(file_name);
			std::string line;

			if (in_file.is_open())
			{
				
				
				if(! getline(in_file, line)) {return false;}
			
				std::vector<std::string> parsefirst = split(line, '\t');
				// ONE_VIDEO_PARAMS cur_video;
				assert(parsefirst.size() == 4);
				cur_video.BITRATE_LEVELS = std::stoi(parsefirst[0]);
				cur_video.TOTAL_VIDEO_CHUNCK = std::stoi(parsefirst[1]);
				cur_video.VIDEO_CHUNCK_LEN = std::stoi(parsefirst[2]) ;
				cur_video.BUFFER_THRESH = (int)std::round(std::stof(parsefirst[3]) / MILLISECONDS_IN_SECOND);
				cur_video.MAX_QUALITY = cur_video.BITRATE_LEVELS - 1;
				cur_video.DEFAULT_QUALITY = cur_video.BITRATE_LEVELS>1 ? 1: 0;
				cur_video.file_name = file_name;

				if(! getline(in_file, line))  {return false;}
				std::vector<std::string> parsesecond = split(line, '\t');
				assert(parsesecond.size() == 12);
				int available_bitrate_count = 0;
				for (int one_hot = 0; one_hot < 12 ; one_hot ++){
					if (  parsesecond[one_hot].compare("1") == 0){
						cur_video.VIDEO_BIT_RATE[available_bitrate_count++] = POSSIBLE_VIDEO_BIT_RATE[one_hot];
					}
				}

				assert(available_bitrate_count == cur_video.BITRATE_LEVELS);
				cur_video.video_size.resize(available_bitrate_count);
				int chunkno;
				for (chunkno = 1 ; chunkno <= cur_video.TOTAL_VIDEO_CHUNCK; chunkno++){
					if(! getline(in_file, line)) {break; }
					std::vector<std::string> parse = split(line, '\t');
					assert(parse.size() == cur_video.BITRATE_LEVELS);

					for (int br = 0 ; br < cur_video.BITRATE_LEVELS ; br++){
						cur_video.video_size[br].push_back((unsigned int)( std::round(std::stof(parse[br]) * 1024 * 1024 )) );
					}

				}
				assert(chunkno-1 == cur_video.TOTAL_VIDEO_CHUNCK);

				// all_video_params.push_back(cur_video);
				// std::cout << cur_video.file_name << "Success" << '\n' ;
				in_file.close();
				return true;				
				
			}

		// }
	// }
	// closedir(dir);

	// return all_video_params;
	return false;
}

float restore_or_compute_download_time(
	std::vector<std::vector<std::vector<float> > >& all_download_time,
	unsigned int video_chunk,
	unsigned int quan_t_idx,
	unsigned int bit_rate,
	std::vector<float>& quan_time, 
	std::vector<float>& quan_bw,
	float dt,
	std::vector<unsigned int>& video_size) {

	if (all_download_time[video_chunk][quan_t_idx][bit_rate] != \
		INVALID_DOWNLOAD_TIME) {
		return all_download_time[video_chunk][quan_t_idx][bit_rate];
	} else {
		unsigned int chunk_size = video_size[video_chunk];
		float downloaded = 0;
		float time_spent = 0;
		unsigned int quan_idx = quan_t_idx;
		while (true) {
			float curr_bw = quan_bw[quan_idx];  // in Mbit/sec
			downloaded += curr_bw * dt / BITS_IN_BYTE * B_IN_MB * PACKET_PAYLOAD_PORTION;
			quan_idx += 1;
			if (downloaded >= chunk_size || quan_idx >= quan_time.size()) {
				break;
			}
			time_spent += dt;  // lower bound the time spent
		}
		all_download_time[video_chunk][quan_t_idx][bit_rate] = time_spent;
		return time_spent;
	}
}


template <typename T>
T must_retrieve(
	std::unordered_map<uint64_t, T>& dict_map,
	unsigned int chunk_idx,
	unsigned int time_idx,
	unsigned int buffer_idx,
	unsigned int bit_rate) {
	uint64_t key = combine((uint64_t) chunk_idx,
						   (uint64_t) time_idx,
						   (uint64_t) buffer_idx,
						   (uint64_t) bit_rate);
	auto it = dict_map.find(key);
	assert (it != dict_map.end());
	return it -> second;
}


template <typename T>
void insert_or_update(
	std::unordered_map<uint64_t, T>& dict_map,
	unsigned int chunk_idx,
	unsigned int time_idx,
	unsigned int buffer_idx,
	unsigned int bit_rate,
	T value) {
	uint64_t key = combine((uint64_t) chunk_idx,
						   (uint64_t) time_idx,
						   (uint64_t) buffer_idx,
						   (uint64_t) bit_rate);
	auto it = dict_map.find(key);
	if (it == dict_map.end()) {
		dict_map.insert({key, value});
	} else {
		it->second = value;
	}
}


template <typename T>
bool insert_or_compare_and_update(
	std::unordered_map<uint64_t, T>& dict_map,
	unsigned int chunk_idx,
	unsigned int time_idx,
	unsigned int buffer_idx,
	unsigned int bit_rate,
	T value) {
	uint64_t key = combine((uint64_t) chunk_idx,
						   (uint64_t) time_idx,
						   (uint64_t) buffer_idx,
						   (uint64_t) bit_rate);
	auto it = dict_map.find(key);
	if (it == dict_map.end()) {
		dict_map.insert({key, value});
		return true;
	} else {
		if (value > it->second) {
			it->second = value;
			return true;
		}
	}
	return false;
}


template <typename T>
bool found_in(
	std::unordered_map<uint64_t, T>& dict_map,
	unsigned int chunk_idx,
	unsigned int time_idx,
	unsigned int buffer_idx,
	unsigned int bit_rate) {
	uint64_t key = combine((uint64_t) chunk_idx,
						   (uint64_t) time_idx,
						   (uint64_t) buffer_idx,
						   (uint64_t) bit_rate);
	auto it = dict_map.find(key);
	if (it == dict_map.end()) {
		return false;
	} else {
		return true;
	}
}

int main() {

	ALL_COOKED_TIME_BW all_cooked_time_bw = \
		get_all_cooked_time_bw(COOKED_TRACE_FOLDER);

	// std::vector< ONE_VIDEO_PARAMS > all_video_params = \
	// 	get_all_video_params(VIDEO_PARAMS_FOLDER);

//
	struct dirent *entry;
	DIR *dir = opendir(VIDEO_PARAMS_FOLDER.c_str());
	if (dir != NULL) {
		int i = 0 ;
		while ((entry = readdir(dir)) != NULL && i<=10) {
			if (entry->d_name[0] == '.') continue;  // read . or ..

			std::string file_name = "";
			file_name += VIDEO_PARAMS_FOLDER;
			file_name += entry->d_name;
			printf("%s\n", file_name.c_str());

			// ONE_VIDEO_PARAMS * cur_video_pointer = new ONE_VIDEO_PARAMS;
			ONE_VIDEO_PARAMS cur_video;
			bool is_parsed = get_one_video_params(file_name , cur_video);
			if(! is_parsed) continue;

	std::ofstream log_file;
	// ONE_VIDEO_PARAMS cur_video = all_video_params[video_idx];

	assert(all_cooked_time_bw.all_cooked_time.size() == \
		all_cooked_time_bw.all_cooked_bw.size());
	assert(all_cooked_time_bw.all_cooked_time.size() == \
		all_cooked_time_bw.all_file_names.size());

	for (unsigned int trace_idx = 0; 
		 trace_idx < all_cooked_time_bw.all_cooked_time.size() && trace_idx < 10; 
		 trace_idx ++) {

		std::vector<float> cooked_time = all_cooked_time_bw.all_cooked_time[trace_idx];
		std::vector<float> cooked_bw = all_cooked_time_bw.all_cooked_bw[trace_idx];
		log_file.open (OUTPUT_FILE_PATH + "_" + cur_video.file_name +  "_" + all_cooked_time_bw.all_file_names[trace_idx]);

		// -----------------------------------------
		// step 1: quantize the time and bandwidth
		// -----------------------------------------

		unsigned int total_time_pt = ceil(cooked_time[cooked_time.size() - 1] / DT);
		
		std::vector<float> quan_time(total_time_pt + 1);
		float d_t = floor(cooked_time[0]);
		for (int i = 0; i <= total_time_pt; i ++) {
			quan_time[i] = d_t;
			d_t += DT;
		}

		std::vector<float> quan_bw(total_time_pt + 1);
		unsigned int curr_time_idx = 0;
		for (int i = 0; i < quan_bw.size(); i ++) {
			while (curr_time_idx < cooked_time.size() - 1 && 
				  cooked_time[curr_time_idx] < quan_time[i]) {
				  	curr_time_idx += 1;
				  }
			quan_bw[i] = cooked_bw[curr_time_idx];
		}
		
		// ----------------------------------------
		// step 2: cap the max time and max buffer
		// ----------------------------------------
		unsigned int max_video_contents = 0;
		for (auto& n : cur_video.video_size[cur_video.BITRATE_LEVELS - 1])
			max_video_contents += n;
		float total_bw = 0;
		for (auto& n : quan_bw)
			total_bw += n;
		total_bw *= DT;  // in Mbit

		float t_portion = max_video_contents / \
			(total_bw * B_IN_MB * PACKET_PAYLOAD_PORTION / BITS_IN_BYTE);

		unsigned int t_max = cooked_time[cooked_time.size() - 1] * t_portion;

		unsigned int t_max_idx = ceil(t_max / DT);
		unsigned int b_max_idx = t_max_idx;

		for (int i = 1; i < ceil(t_portion); i ++) {
			float last_quan_time = quan_time[quan_time.size() - 1];
			for (int j = 1; j <= total_time_pt; j ++ ) {
				quan_time.push_back(quan_time[j] + last_quan_time);
				quan_bw.push_back(quan_bw[j]);
			}
		}

		if (quan_time[quan_time.size() - 1] < t_max) {
			// expand quan_time has 1 idx less (0 idx is 0 time)
			// precaution step
			float last_quan_time = quan_time[quan_time.size() - 1];
                        for (int j = 1; j <= total_time_pt; j ++ ) {
                                quan_time.push_back(quan_time[j] + last_quan_time);
                                quan_bw.push_back(quan_bw[j]);
                        }
		}
		
		assert(quan_time[quan_time.size() - 1] >= t_max);

		// -----------------------------------------------------------
		// (optional) step 3: pre-compute the download time of chunks
		// download_time(chunk_idx, quan_time, bit_rate)
		// -----------------------------------------------------------

		std::vector<std::vector<std::vector<float> > > all_download_time(
			cur_video.TOTAL_VIDEO_CHUNCK,
			std::vector<std::vector<float> >(
				t_max_idx,
				std::vector<float> (cur_video.BITRATE_LEVELS)));

		for (int i = 0; i < cur_video.TOTAL_VIDEO_CHUNCK; i ++) {
			for (int j = 0; j < t_max_idx; j ++) {
				for (int k = 0; k < cur_video.BITRATE_LEVELS; k ++) {
					// means not visited before
					all_download_time[i][j][k] = INVALID_DOWNLOAD_TIME;
					restore_or_compute_download_time(
										all_download_time, i, j, k,
										quan_time, quan_bw, DT,cur_video.video_size[k]);
				}
			}
		}

		// -----------------------------
		// step 4: dynamic programming
		// -----------------------------
		std::vector<std::unordered_map<uint64_t, float> > total_reward( cur_video.TOTAL_VIDEO_CHUNCK,
			std::unordered_map<uint64_t, float>());

		std::unordered_map<uint64_t, DP_PT> last_dp_pt;

		float download_time = restore_or_compute_download_time(
			all_download_time, 0, 0, cur_video.DEFAULT_QUALITY, 
			quan_time, quan_bw, DT, cur_video.video_size[cur_video.DEFAULT_QUALITY]);

		float chunk_finish_time = download_time + LINK_RTT / M_IN_K;
		unsigned int time_idx = floor(chunk_finish_time / DT);
		unsigned int buffer_idx = int(cur_video.VIDEO_CHUNCK_LEN / M_IN_K / DT);

		float reward = cur_video.VIDEO_BIT_RATE[cur_video.DEFAULT_QUALITY] / M_IN_K \
			- REBUF_PENALTY * chunk_finish_time;

		insert_or_update(total_reward[0],
			0, time_idx, buffer_idx, cur_video.DEFAULT_QUALITY,
			reward);

		DP_PT dp_pt = {0, 0, 0, 0};
		insert_or_update(last_dp_pt, 
			0, time_idx, buffer_idx, cur_video.DEFAULT_QUALITY,
			dp_pt);

		for (unsigned int n = 1; n < cur_video.TOTAL_VIDEO_CHUNCK; n ++) {
			std::cout << n << " " << cur_video.TOTAL_VIDEO_CHUNCK << '\n';
			float max_reward_up_to_n = -INFINITY;
			float max_reward_remaining_after_n = (cur_video.TOTAL_VIDEO_CHUNCK - n - 1) * cur_video.VIDEO_BIT_RATE[cur_video.MAX_QUALITY] / M_IN_K;
			
			for (auto it = total_reward[n-1].begin(); it != total_reward[n-1].end(); it ++) {
				// extract elements from key
				uint64_t key = it->first;
				uint64_t mask = 65536 - 1;
				unsigned int m = key & mask;
				unsigned int b = (key >> 16) & mask;
				unsigned int t = (key >> 32) & mask;
				float boot_strap_reward = it->second;
				//unsigned int chunk_num = (key >> 48) & mask;
				//assert (chunk_num == n - 1);

				for (unsigned int nm = 0; nm < cur_video.BITRATE_LEVELS; nm ++) {
					download_time = all_download_time[n][t][nm];

					float buffer_size = quan_time[b];
					float rebuf = download_time - buffer_size;
					if (rebuf < 0) rebuf = 0;

					reward = cur_video.VIDEO_BIT_RATE[nm] / M_IN_K \
							- REBUF_PENALTY * rebuf \
							- SMOOTH_PENALTY * abs(
							cur_video.VIDEO_BIT_RATE[nm] -
							cur_video.VIDEO_BIT_RATE[m]) / M_IN_K;

					buffer_size = buffer_size - download_time - LINK_RTT / M_IN_K;
					if (buffer_size < 0) buffer_size = 0;

					buffer_size += cur_video.VIDEO_CHUNCK_LEN / M_IN_K;

					float drain_buffer_time = 0;
					if (buffer_size > cur_video.BUFFER_THRESH) {
						// exceed the buffer limit
						drain_buffer_time = buffer_size - cur_video.BUFFER_THRESH;
						buffer_size = cur_video.BUFFER_THRESH;
					}

					buffer_idx = ceil(buffer_size / DT);

					chunk_finish_time = quan_time[t] + \
						download_time + drain_buffer_time + LINK_RTT / M_IN_K;

					time_idx = floor(chunk_finish_time / DT);

					reward += boot_strap_reward;
					max_reward_up_to_n = (reward > max_reward_up_to_n)? reward:max_reward_up_to_n;

					if (reward + max_reward_remaining_after_n >= max_reward_up_to_n - 0.01) {
						bool updated = insert_or_compare_and_update(
										total_reward[n],
										n, time_idx, buffer_idx, nm,
										reward);

						if (updated) {
							dp_pt = (DP_PT){n - 1, t, b, m};
							insert_or_update(last_dp_pt, 
											n, time_idx, buffer_idx, nm,
											dp_pt);
						}
					}					
				}
			}
		}

		unsigned int total_keys = 0;
		for (int n = 0; n < cur_video.TOTAL_VIDEO_CHUNCK; n++)
			total_keys += total_reward[n].size(); 
		
		std::cout << "total keys = " << total_keys << std::endl;
		std::cout << "max keys = " << cur_video.TOTAL_VIDEO_CHUNCK * t_max_idx * b_max_idx * cur_video.BITRATE_LEVELS << std::endl; 

		// ---------------------------------
		// step 5: get the max total reward
		// ---------------------------------
		float optimal_total_reward = -std::numeric_limits<float>::max();  // should be -Infty, not +0. (M's bug)
		dp_pt = (DP_PT){0, 0, 0, 0};


		unsigned last_time_idx = 0;
		unsigned last_buff_idx = 0;
		unsigned last_bit_rate = 0;

		for (auto it = total_reward[cur_video.TOTAL_VIDEO_CHUNCK-1].begin(); it != total_reward[cur_video.TOTAL_VIDEO_CHUNCK-1].end(); it ++) {
			// extract elements from key
			uint64_t key = it->first;
			uint64_t mask = 65536 - 1;
			unsigned int m = key & mask;
			unsigned int b = (key >> 16) & mask;
			unsigned int t = (key >> 32) & mask;
			float tmp_total_reward = it->second; 
			if (tmp_total_reward > optimal_total_reward) {
					optimal_total_reward = tmp_total_reward;
					dp_pt = must_retrieve(
							last_dp_pt, cur_video.TOTAL_VIDEO_CHUNCK - 1, t, b, m);

					last_time_idx = t;
					last_buff_idx = b;
					last_bit_rate = m;
			}
		}

		std::ofstream output;
		log_file << optimal_total_reward << '\n';

		log_file << cur_video.TOTAL_VIDEO_CHUNCK - 1 << '\t' \
				 << last_time_idx << '\t' \
				 << last_buff_idx << '\t' \
				 << quan_time[last_time_idx] << '\t' \
				 << quan_time[last_buff_idx] << '\t' \
				 << quan_bw[last_time_idx] << '\t' \
				 << last_bit_rate << '\n';

		while (dp_pt.chunk_idx != 0 || dp_pt.time_idx != 0 ||
			dp_pt.buffer_idx != 0 || dp_pt.bit_rate != 0) {
			log_file << dp_pt.chunk_idx << '\t' \
					 << dp_pt.time_idx << '\t' \
					 << dp_pt.buffer_idx << '\t' \
					 << quan_time[dp_pt.time_idx] << '\t' \
					 << quan_time[dp_pt.buffer_idx] << '\t' \
					 << quan_bw[dp_pt.time_idx] << '\t' \
					 << dp_pt.bit_rate << '\n';
			dp_pt = must_retrieve(
				last_dp_pt, dp_pt.chunk_idx, dp_pt.time_idx,
				dp_pt.buffer_idx, dp_pt.bit_rate);
		}
		log_file << '\n';
		log_file.close();

	} // end of traces loop
		std::cout << cur_video.file_name << '\n' << '\n'<< '\n'<< '\n';

		i++;
		} // end of video loop
	}
	closedir(dir);
//
	// for(unsigned int video_idx = 0;
	// 	video_idx < all_video_params.size() && video_idx < 10;
	// 	video_idx++
	// )
	// {		
	// // std::vector<std::vector<unsigned int> > video_sizes = \
	// // 	get_video_sizes(VIDEO_SIZE_FILE);


	// } // end of video loop
} // end of main
