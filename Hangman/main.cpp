//
//  main.cpp
//  Hangman
//
//  Created by xin cui on 14/10/29.
//  Copyright (c) 2014年 xin cui. All rights reserved.
//

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

using namespace std;

vector<string> words{"E","T","A","O","I","N","S","H","R","D","L","C","U","M","W","F",
                     "G","Y","P","B","V","K","J","X","Q","Z"};

string post_request(string data){
    using namespace curlpp;
    
    stringstream out;
    stringstream ss;
    ss << "http://strikingly-interview-test.herokuapp.com/guess/process";
    
    try {
        curlpp::Cleanup clean;
        curlpp::Easy request;
        
        curlpp::options::WriteStream ws(&out);
        request.setOpt(ws);
        request.setOpt<options::Url>(ss.str());
        list<string> header;
        header.push_back("Content-Type:application/json");
        request.setOpt(new curlpp::options::HttpHeader(header));
        
        request.setOpt(new curlpp::options::PostFields(data));
        request.setOpt(new curlpp::options::PostFieldSize(data.length()));
        request.perform();
        
    } catch (curlpp::RuntimeError &e) {
        cerr << e.what() << endl;
        return "";
    }
    string body = out.str();
    int idx = body.find("\"secret\"");
    if(idx != -1)
        return body.substr(idx, 41);
    return "";

}

string give_me_a_word(string secret){
    
    string data = "{\"userId\":\"i@arriettyc.com\",\"action\":\"nextWord\"," + secret + "}";
    
    using namespace curlpp;
    
    stringstream out;
    stringstream ss;
    ss << "http://strikingly-interview-test.herokuapp.com/guess/process";
    
    try {
        curlpp::Cleanup clean;
        curlpp::Easy request;
        
        curlpp::options::WriteStream ws(&out);
        request.setOpt(ws);
        request.setOpt<options::Url>(ss.str());
        list<string> header;
        header.push_back("Content-Type:application/json");
        request.setOpt(new curlpp::options::HttpHeader(header));
        
        request.setOpt(new curlpp::options::PostFields(data));
        request.setOpt(new curlpp::options::PostFieldSize(data.length()));
        request.perform();
        
    } catch (curlpp::RuntimeError &e) {
        cerr << e.what() << endl;
        return "";
    }
    cerr << out.str() << endl;
    string body = out.str();
    int start = body.find("\"word\"");
    int end = body.find("\"userId\"");
    
    string info = body.substr(start + 8, end - start - 10);
    return info;
}

void make_a_guess(string info, string secret){
    
    string data;
    using namespace curlpp;
    
    stringstream out;
    stringstream ss;
    ss << "http://strikingly-interview-test.herokuapp.com/guess/process";
    
    try {
        curlpp::Cleanup clean;
        curlpp::Easy request;
        
        request.setOpt(curlpp::options::WriteStream (&out));
        request.setOpt<options::Url>(ss.str());
        list<string> header;
        header.push_back("Content-Type:application/json");
        request.setOpt(new curlpp::options::HttpHeader(header));
        
        int count = 10, idx = 0;
        int len = info.length();
        string body;
        
        while(info.find("*") != -1 && count > 0){
            
            data = "{\"action\":\"guessWord\",\"guess\":\"" + words[idx++] +"\",\"userId\":\"i@arriettyc.com\"," + secret + "}";
            //cout << data << endl;

            request.setOpt(new curlpp::options::PostFields(data));
            request.setOpt(new curlpp::options::PostFieldSize(data.length()));
            request.perform();
            
            body = out.str();
            out.str("");
            
            int start = body.find("\"word\"");
            
            info = body.substr(start + 7, len);
            count--;
            //cerr << data << endl;
            //cerr << "********" << body << endl;
        }
        cerr << body << endl;
        
        
        
    } catch (curlpp::RuntimeError &e) {
        cerr << e.what() << endl;
        return ;
    }
    
    
}

int get_test_results(string secret){
    
    string data = "{\"action\":\"getTestResults\",\"userId\":\"i@arriettyc.com\"," + secret + "}";;
    
    using namespace curlpp;
    stringstream out;
    stringstream ss;
    ss << "http://strikingly-interview-test.herokuapp.com/guess/process";
    
    try {
        curlpp::Cleanup clean;
        curlpp::Easy request;
        
        curlpp::options::WriteStream ws(&out);
        request.setOpt(ws);
        request.setOpt<options::Url>(ss.str());
        list<string> header;
        header.push_back("Content-Type:application/json");
        request.setOpt(new curlpp::options::HttpHeader(header));
        
        request.setOpt(new curlpp::options::PostFields(data));
        request.setOpt(new curlpp::options::PostFieldSize(data.length()));
        request.perform();
        
        string body = out.str();
        int idx = body.find("totalScore");
        string score = body.substr(idx + 12, body.length() - idx - 14);
        cerr << body << endl;
        return stoi(score);
        
        
    } catch (curlpp::RuntimeError &e) {
        cerr << e.what() << endl;
        return -1000;
    }
    return -1000;
    
}

struct result{
    string secret;
    int score;
}R;


void get_the_highest_score(string secret, int score){
    if(score > R.score){
        R.score = score;
        R.secret = secret;
    }
    return ;
}


int main(int argc, const char * argv[]) {
    //freopen("out.txt", "w",stdout);
    
    while(true){
        string data = "{\"action\" : \"initiateGame\", \"userId\" : \"i@arriettyc.com\"}";
        string secret = post_request(data);
        //cout << secret << endl;
    
        int counts = 80;
        while (counts--) {
            string info = give_me_a_word(secret);
            make_a_guess(info, secret);
        }


        int score = get_test_results(secret);
        cout << secret << " " << score << endl;
    }
    return 0;
}
