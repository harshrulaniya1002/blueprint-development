#include<iostream>
#include<string>
using namespace std;


vector<int> LPS(string& text){
    int n=text.size();
    vector<int>lps(n,0);

    int len=0;
    int i=1;
    
    while(i<n){
        if(text[len] == text[i]){
            len++;
            lps[i]=len;
            i++;
        }else{
            if(len>0){
                len=lps[len-1];
            }else{
                lps[i]=0;
                i++;
            }
        }
    }

    for(auto it : lps){
        cout<<it<<" ";
    }
    cout<<"\n";
    return lps;
}

vector<int> pattExistInText(vector<int>&prefsuff , string& text, string& patt){
    vector<int>ans;

    int m=patt.size();
    int n=text.size();

    int i=0;
    int j=0;

    while(i<n){
        if(text[i] == patt[j]){
            i++;
            j++;
        }

        if(j == m){
            ans.push_back(i-j);
            j=prefsuff[j-1];
        }else if(i< n && text[i] != patt[j]){
            if(j!= 0){
                j = prefsuff[j-1];
            }else{
                i++;
            }
        }
    }

    return ans;
}


int main(){
    string patt,text;
    cin>>patt>>text;
    vector<int> prefsuff = LPS(text);
    vector<int>matchingIdx = pattExistInText(prefsuff,text,patt);

    cout<<"pattern matches at indexes"<<"\n";
    if(matchingIdx.size() > 0){
        for(auto it: matchingIdx){
            cout<<it<<" ";
        }
        cout<<"\n";
    }
}