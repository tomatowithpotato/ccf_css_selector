#include <stdio.h>
#include <string>
#include <cctype>
#include <unordered_map>
#include <vector>
#include <queue>
#include <utility>
#include <set>
#include <iostream>
#include <algorithm>

using namespace std;

struct element{
    string tag;
    string id;
    int level;
    element(const string &t = "", const string &i = "", int l = 0){
        tag = t;
        id = i;
        level = l;
    }
};

int N, M;
vector<string> raw_doc;
vector<element> doc;

bool tag_compare(const string &a, const string &b){
    if(a.size() != b.size())return false;
    int n = a.size();
    for(int i = 0; i < n; i++){
        if(isalpha(a[i]) && isalpha(b[i])){
            if(toupper(a[i]) != toupper(b[i])){
                return false;
            }
        }
        else if(a[i] != b[i]){
            return false;
        }
    }
    return true;
}

void structure(){
    for(int i = 0; i < N; i++){
        int level;
        string tag, id;
        int j = 0, n = raw_doc[i].size();
        while(j < n && raw_doc[i][j] == '.')++j;
        level = j;
        while(j < n && raw_doc[i][j] != ' '){
            tag.push_back(raw_doc[i][j++]); 
        }
        ++j; //跳过空格
        while(j < n){
            id.push_back(raw_doc[i][j++]);
        }
        doc.push_back(element(tag, id, level));
    }
}

bool query(const string &selector, vector<int> &res, int n, int row, int level, int pos){
    bool find_out = false;
    if(pos >= n){
        res.push_back(row);
        return true;
    }
    string now;
    while(pos < n && selector[pos] != ' '){
        now.push_back(selector[pos++]);
    }
    for(; row < N && doc[row].level > level; row++){
        bool f = false;
        if(now[0] == '#' && now == doc[row].id){
            f |= query(selector, res, n, row+1, doc[row].level, pos+1);
        }
        else if(tag_compare(now,  doc[row].tag)){
            f |= query(selector, res, n, row+1, doc[row].level, pos+1);
        }
        //如果当前不是最后一层，且当前元素在当前层级匹配成功，则不需要在当前层级匹配他的后代元素了，以防重复
        if(f && pos < n){
            int lev = doc[row].level;
            for(++row; row < N && doc[row].level > lev; row++);
            --row;
        }
        find_out |= f;
    }
    return find_out;
}

int main(){
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    cin >> N >> M;
    string s;
    getline(cin, s); //去缓存
    for(int i = 0; i < N; i++){
        getline(cin, s);
        raw_doc.push_back(s);
    }
    structure();
    
    for(int i = 0; i < M; i++){
        vector<int> res;
        getline(cin, s);
        query(s, res, s.size(), 0, -1, 0);
        cout << res.size() << " ";
        for(auto i : res){
            cout << i << " ";
        }
        cout << endl;
    }
    
    return 0;
}