#include<bits/stdc++.h>
#include<unordered_map>
using namespace std;


int BOUGHT_TOP = 25;

typedef long long LL;
typedef pair<LL, LL> PLL;
typedef pair<double, LL> PDL;
//#define DEBUG

//#include <tr1/unordered_map>
//using std::tr1::unordered_map;


LL M = 1LL<<32;
int SHOW = 100000;
int OUTNUM = 200;
int DEBUG_LIMIT = 200000;
double eps = 1e-6;


inline LL haxi(LL a, LL b){
    if(a > b) swap(a, b);
    return a * M + b;
}

unordered_map<LL, vector<PDL> > item_vector;
unordered_map<LL, double> item_vector_len;
//base function
inline double len(vector<PDL>& v){
    double sum = 0;
    for(int i = 0; i < v.size(); i++)
        sum += v[i].first * v[i].first;
    return sqrt(sum);
}
inline double similarity(LL a, LL b){
    vector<PDL> &va = item_vector[a];
    vector<PDL> &vb = item_vector[b];
    unordered_map<LL, double> ub; 
    for(int i = 0; i < vb.size(); i++)
        ub[vb[i].second] = vb[i].first;
    double sum = 0.0;
    for(int i = 0; i < va.size(); i++)
        sum += ub[va[i].second] * va[i].first;
    return sum / item_vector_len[a] / item_vector_len[b];
}
double similarity(vector<PDL>& a, vector<PDL>& b){
    double la = 0, lb = 0, sum = 0;
    unordered_map<LL, double> mld;
    for(int i = 0; i < a.size(); i++){
        la += a[i].first * a[i].first;
        mld[a[i].second] = a[i].first;
    }
    for(int i = 0; i < b.size(); i++){
        lb += b[i].first * b[i].first;
        sum += mld[b[i].second] * b[i].first;
    }
    la = sqrt(la), lb = sqrt(lb);
    return sum / la / lb;
}

template<class T>
void lxunique(vector<T> &v, string who = ""){
    cerr<<who<<" before size = \t"<<v.size()<<"\t";
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
    cerr<<who<<" after size = \t"<<v.size()<<endl;
}


// step zero, user history cal, unique
unordered_map<LL, vector<LL> > item_buyers;
unordered_map<LL, vector<LL> > buyer_items;
void gao_item_buyers(){
    fstream buy_file("../data/user_bought_history.txt");
    LL user, item, day;
    int now = 0, tot = 0;
    while(buy_file >> user >> item >> day){
        if(now ++ % SHOW == 0)
            cerr<<"user\t"<<user<<"\titem\t"<<item<<"\tday\t"<<day<<endl;
        item_buyers[item].push_back(user);
        buyer_items[user].push_back(item);
    }

    // erase same elements
    for(unordered_map<LL, vector<LL> >::iterator it = item_buyers.begin(); it != item_buyers.end(); it++){
        vector<LL> &v = it->second;
        lxunique(v, "item_buyers");
    }
    for(unordered_map<LL, vector<LL> >::iterator it = buyer_items.begin(); it != buyer_items.end(); it++){
        vector<LL> &v = it->second;
        lxunique(v, "buyer_items");
    }
}



//step one cal all TF-IDF vector
unordered_map<LL, vector<LL> > item_values;
unordered_map<LL, LL> item_cat;
unordered_map<LL, vector<LL> > cat_items;
unordered_map<LL, unordered_map<LL, int> > item_value_times;
unordered_map<LL, int> value_times;
void gao_TF_IDF(char * dim_items_txt){
    fstream item_values_file(dim_items_txt);              // for item's value
    int tot = 0, now = 0;
    // get item's value vector --------
    LL item, cat; string values_string;
    double st = clock();
    while(item_values_file >> item >> cat >> values_string){
        assert(item < M);
        if (now++ % SHOW == 0)
            cerr<<"now:\t"<<now<<"\titem:\t"<<item<<"\tcat\t"<<cat<<"\tvs\t"<<values_string<<endl;
        vector<LL> values;
        stringstream ss(values_string);
        LL val; char douhao;
        while(ss >> val){
            ss >> douhao;
            assert(douhao == ',' || douhao == '\n');
            values.push_back(val);
            item_value_times[item][val]++;
        }

        if ((now - 1) % SHOW == 0){ for (int i = 0; i < values.size(); i++) cerr<<" "<<values[i]; cerr<<"\n"; }

        sort(values.begin(), values.end());
        //values.erase(unique(values.begin(), values.end()), values.end());
        int sz = unique(values.begin(), values.end()) - values.begin();
        for (int i = 0; i < sz; i++){ // must unique for idf
            value_times[values[i]]++;
        }
        item_values[item] = values;
        item_cat[item]    = cat;
        cat_items[cat].push_back(item);
        tot++;
    }
    now = 0;
    for (unordered_map<LL, vector<LL> >::iterator it = item_values.begin(); it != item_values.end(); it++){
        LL item = it->first;
        vector<LL> & v = it->second;
        if (now++ % SHOW == 0)
            cerr<<"now:\t"<<now<<"\titem:\t"<<item<<"\tvaluesize:\t"<<v.size()<<endl;
        vector<PDL> res;
        sort(v.begin(), v.end());
        int sz = unique(v.begin(), v.end()) - v.begin();
        int words = v.size();
        for (int i = 0; i < sz; i++){
            LL val = v[i];
            double tf = item_value_times[item][val] * 1.0 / words;
            double idf = log(tot * 1.0 / (1 + value_times[val]));
            res.push_back(PDL(tf * idf, val));
            if ((now - 1) % SHOW == 0) { cerr<<"tf-idf = "<<tf*idf<<"\tval = "<<val<<endl; }
        }
        item_vector[item] = res;
        item_vector_len[item] = len(res);
    }
    cout<<"get item's vector over, time use:\t"<<(clock() - st) / CLOCKS_PER_SEC<<"\n";
    item_value_times.clear();
    value_times.clear();
}


// xxx_train_item_item.txt -> same[item] = [(1.0, item), (0.9, item_other)]
unordered_map<LL, vector<PLL> > train_cat_item_item; // trainset one cat has (a, b), cat is a's cat
unordered_map<LL, vector<PDL> > same;           // same above
unordered_map<LL, bool> train_item_map;
unordered_map<LL, bool> cat_cat_map;
void gao_same_list(char * xxx_train_item_item){
    fstream item_item_file(xxx_train_item_item);
    double st = clock();
    LL a, b;
    int now = 0;
    vector<LL> train_items;
    while(item_item_file>>a>>b){
        // item a, b
        if (now++ % SHOW == 0)
            cerr<<"now:\t"<<now<<"\titema:\t"<<a<<"\titemb:\t"<<b<<endl;
        train_items.push_back(a);
        train_items.push_back(b);
        LL ca = item_cat[a], cb = item_cat[b];
        cat_cat_map[haxi(ca, cb)] = true;
        train_cat_item_item[ca].push_back(PLL(a, b));
        train_cat_item_item[cb].push_back(PLL(b, a));
        train_item_map[a] = 1;
        train_item_map[b] = 1;
    }

    // reduce cat's items && train_items
    lxunique(train_items);

    for (unordered_map<LL, vector<PLL> >::iterator it = train_cat_item_item.begin(); it != train_cat_item_item.end(); it++){
        vector<PLL> & v = it->second;
        lxunique(v);
    }

    // cat_items
    now = 0;
    cerr<<"train_item num = "<<train_items.size()<<endl;
    for (int i = 0; i < train_items.size(); i++){
    //for (int i = 0; i < 1000; i++){
        LL train_item = train_items[i]; // same[train_item]
        LL train_item_cat = item_cat[train_item];
        vector<LL> & same_cat_items = cat_items[train_item_cat];
        if (now++ % 200 == 0)
            cerr<<"now:\t"<<now<<"\ttrain_item =\t"<<train_item<<"\tcheck items number =\t"<<same_cat_items.size()<<endl;

        vector<PDL> res;
        for(int j = 0; j < same_cat_items.size(); j++){
            LL check_item = same_cat_items[j];
            // train_item vs check_item
            // if (check_item == train_item) continue;
            double similar = similarity(train_item, check_item);
            res.push_back(PDL(similar, check_item));
        }

        sort(res.begin(), res.end());
        reverse(res.begin(), res.end());


        if ((now - 1) % 200 == 0){
            cerr<<"train_item = "<<train_item<<endl;
            for (int i = 0; i < 20; i++){
                cerr<<res[i].first<<" "<<res[i].second<<endl;
            }
        }
        
        same[train_item] = res;
    }
}


// from train_cat_item_item to find better match one
void gao_test(char * test_file_txt, char * output_file_txt){
    fstream item_online_test_file(test_file_txt);
    ofstream item_output_file(output_file_txt);
    
    double st = clock();
    int now = 0, SHOW = 1;
    LL test_item;
    while(item_online_test_file >> test_item){
        vector<PDL> ans;
        unordered_map<LL, bool> has;
        LL bias = 100000;
    

        double sstt = clock();
        if(now++ % SHOW == 0)
            cerr<<"now:\t"<<now<<endl;

        
// add most buyers for first choice
        vector<PLL> vec_buyer;// (buyertimes, item)
        unordered_map<LL, int> item_bought_times;
        vector<LL> & buyers = item_buyers[test_item];
        cerr<<"check buyer number = "<<buyers.size()<<endl;
        for(int i = 0; i < buyers.size(); i++){
            LL buyer = buyers[i];
            vector<LL> & bought_items = buyer_items[buyer];
            for(int j = 0; j < bought_items.size(); j++){
                LL another_item = bought_items[j];
                if (another_item == test_item) 
                    continue;
                LL aic = item_cat[another_item], 
                   tic = item_cat[test_item];
                //if(cat_cat_map[haxi(aic, tic)]) // if logical reasonable, cat-cat can be matched
                if(cat_cat_map.find(haxi(aic, tic)) != cat_cat_map.end())
                    item_bought_times[another_item]++;
            }
        }
        for(unordered_map<LL, int>::iterator it = item_bought_times.begin(); it != item_bought_times.end(); it++){
            vec_buyer.push_back(PLL(it->second, it->first));
        }
        sort(vec_buyer.begin(), vec_buyer.end());
        reverse(vec_buyer.begin(), vec_buyer.end());
        int sz = vec_buyer.size(), max_bought = 0;
        if (sz > 0){
            cerr<<"max buyer number:\t"<<vec_buyer[0].first<<"\tmaxitem:\t"<<vec_buyer[0].second<<"\tmin buyer number:\t"<<vec_buyer[sz - 1].first<<"\tminitem:\t"<<vec_buyer[sz - 1].second<<endl;
            max_bought = vec_buyer[0].first;
        }
        for(int i = 0; i < vec_buyer.size(); i++){
            if((int)ans.size() >= BOUGHT_TOP)
                break;
            LL item = vec_buyer[i].second;
            int times = vec_buyer[i].first;
            ans.push_back(PLL(bias * times, item));
            has[item] = true;
        }
        cerr<<"total item bought number = "<<ans.size()<<endl;
        

// prob similarity search
        LL test_item_cat = item_cat[test_item];
        vector<PLL> &vp = train_cat_item_item[test_item_cat];
        vector<double> prob_left(vp.size());
        vector<vector<PDL> > vvp(vp.size());
        for (int i = 0; i < vp.size(); i++){
            vvp[i] = same[vp[i].second];
            prob_left[i] = similarity(test_item, vp[i].first);
        }
        vector<int> pt(vvp.size(), 0);
        for (int c = 0; ; c++){
            if((int)ans.size() >= OUTNUM)
                break;
            int w = -1;
            double max_prob = -1111111;
            LL choose_item = -1;
            bool check = false;
            for(int i = 0; i < vvp.size(); i++){
                int p = pt[i];
                vector<PDL>& vpdl = vvp[i];
                if (p >= (int) vpdl.size()) continue;
                check = true;
                double prob = prob_left[i] * vpdl[p].first;
                if (prob > max_prob){
                    max_prob = prob;
                    w = i;
                    choose_item = vpdl[p].second;
                }
            }
            if(check == false) break;
            pt[w]++;
            if(has.find(choose_item) == has.end()){
                ans.push_back(PDL(max_prob, choose_item));
                has[choose_item] = true;
            }
        }

        // sometime no need to sort
        //sort(ans.begin(), ans.end());
        //reverse(ans.begin(), ans.end());

        // write into file
        if((now - 1)% SHOW == 0){
            for(int i = 0; i < 20; i++)
                cerr<<ans[i].first<<"\t"<<ans[i].second<<"\tin train: "<<train_item_map[ans[i].second]<<endl;
            int last = ans.size();
            cerr<<"last\t" <<ans[last - 1].first<<" "<<ans[last - 1].second<<endl;
        }
        cerr<<"time use = "<<(clock() - sstt) / CLOCKS_PER_SEC<<endl;

        item_output_file<<test_item<<" ";
        for(int i = 0; i < OUTNUM - 1; i++){
            item_output_file<<ans[i].second<<",";
        }
        item_output_file<<ans[OUTNUM - 1].second<<"\n";
    }
    cout<<"get online test over, time use:\t"<<(clock() - st) / CLOCKS_PER_SEC<<"\n";
}

int main(int arg, char * argv[]){
    /*
     * ../data/dim_items.txt
     * ./offline_train_item_item.txt
     * ../data/offline_test_items.txt
     * ../submit/offline_ans_20151006.txt
     * */
    gao_item_buyers();
    gao_TF_IDF(argv[1]);
    gao_same_list(argv[2]);
    gao_test(argv[3], argv[4]);
    return 0;
}
