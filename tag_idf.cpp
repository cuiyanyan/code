#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
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
    //std::cout << (int)tag_set.size() << std::endl;
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
int main(){
    long int n = 531164801;
    std::string line;
    std::vector<std::pair<std::string, int> > vec_seg;
    std::map<std::string, int> tag_set;
    std::map<std::string, int> temp;
    std::vector<std::string> result;
    std::map<std::string, int>::iterator tag_it;
    std::map<std::string, int>::iterator it;
    if(initSegment("tc_data") != 0){
        return -1;
    }
    loadDict(tag_set);
    while(std::cin.good()){
        if(!getline(std::cin, line)){
            break;
        }
        vec_seg.clear();
        result.clear();
        result = split(line, "\t");
        if(result.size() < 2){
            continue;
        }
        vec_seg = segmentation(result[1]);
        temp.clear();
        for(int i = 0; i < (int)vec_seg.size(); i++){
            ++temp[vec_seg[i].first];
        }
        it = temp.begin();
        while(it != temp.end()){
            tag_it = tag_set.find(it->first);
            if(tag_it != tag_set.end()){
                tag_it->second += 1;
            }
            ++it;
        }
    }
    tag_it = tag_set.begin();
    while(tag_it != tag_set.end()){
        double idf;
        idf = log((double)n/(tag_it->second+1));
        std::cout << tag_it->first << "\t" << idf << std::endl;
        ++tag_it;
    }
    std::vector<std::pair<std::string, int> >().swap(vec_seg);
    std::map<std::string, int>().swap(tag_set);
    std::map<std::string, int>().swap(temp);
    std::vector<std::string>().swap(result);
    unInitSegment();
    return 0;
}
