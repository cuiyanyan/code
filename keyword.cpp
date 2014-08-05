#include <iostream>
#include <fstream>
#include <map>
#include <cstdlib>
#include <vector>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "word_segment.h"

int loadDict(std::map<std::string, int> & tag_set){
    std::string line;
    std::ifstream input("tag");
    tag_set.clear();
    while(getline(input, line)){
        if(line.empty()){
            continue;
        }
        tag_set.insert(std::make_pair(line, 0));
    }
    input.close();
    return 0;
}

std::vector<std::string> split(std::string str, std::string pattern){
    int pos;
    std::vector<std::string> result;
    str += pattern;
    int size = str.size();
    for(int i = 0; i < size; i++){
        pos = str.find(pattern, i);
        if(pos < size){
            std::string s = str.substr(i, pos-i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}

int main(int argc, char **argv) {
    //std::string key;
    std::string line;
    std::string value;
    std::string dict_dir = "tc_data";
    std::map<std::string, int> tag_set;
    loadDict(tag_set);
    std::map< uint32_t, std::map<std::string, uint32_t> > cate_word_dict;
    if(initSegment("tc_data") != 0){
        return -1;
    }
    std::map<std::string, int>::iterator it;
    std::vector<std::pair<std::string, int> > vec_seg;
    std::vector<std::string> result;
    while (std::cin.good()) {
        if(!getline(std::cin, line))
            break;
        result.clear();
        result = split(line, "\t");
        if((int)result.size() != 3)
            continue;
        uint32_t category_id;
        category_id = std::atoi(result[2].c_str());
        value = result[1];
        vec_seg = segmentation(value);
        for (int32_t j = 0; j < (int)vec_seg.size(); j++) {
            it = tag_set.find(vec_seg[j].first);
            if(it == tag_set.end())
                continue;
            cate_word_dict[category_id][vec_seg[j].first]++;
        }
    }
    unInitSegment();
    std::map< uint32_t, std::map<std::string, uint32_t> >::iterator cate_word_iter;
    std::map<std::string, uint32_t>::iterator word_iter;
    for (cate_word_iter = cate_word_dict.begin();
            cate_word_iter != cate_word_dict.end();
            cate_word_iter++) {
        std::cout << cate_word_iter->first << std::endl;
        for (word_iter = cate_word_iter->second.begin();
                word_iter != cate_word_iter->second.end();
                word_iter++) {
            std::cout << word_iter->first << "\3" << word_iter->second << "\t";
        }
        std::cout << std::endl;
    }
    std::map<std::string, int>().swap(tag_set);
    std::vector<std::pair<std::string, int> >().swap(vec_seg);
    std::map< uint32_t, std::map<std::string, uint32_t> >().swap(cate_word_dict);
    std::vector<std::string>().swap(result);
}
