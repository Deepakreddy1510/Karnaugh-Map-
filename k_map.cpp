#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <bitset>
#include <algorithm>

using namespace std;

int num_vars; // Used to store the number of input variables

// Converts string to binary string of length num_vars
string toBinary(int n){
    return bitset<5>(n).to_string().substr(5 - num_vars);
}

//Check if the two terms(strings) differ by only one bit
bool isComb(const string& a,const string& b,string& res){
    int diff = 0; 
    res = a;
    for(int i=0; i < a.size(); ++i){
    	if(a[i] != b[i]){
    	   ++diff;
	   res[i] = '-';
	}
    }
    return (diff == 1); // We return true if the two terms differ only by 1 bit , like example "1101" and "1111"
}

// Used to build the prime implicant chart to see which prime implicants cover which original min terms
bool covers(const string& a,const string& b){
    for(int i=0; i < a.size(); ++i){
    	if(a[i] != '-' && a[i] != b[i]) return false;
    }	
    return true;
}

// Generate literal expression from binary with dashes
// This is used when finally printing the simplified SOP form expression
string toLiteral(const string& bin){
    string res = "";
    for(int i=0; i < bin.size(); i++){
    	if(bin[i] == '-') continue;
    	res += ('A' + i);
    	if(bin[i] == '0') res += '\'';
    }
    return res;
} 

// Quine–McCluskey Minimization
vector<string> minimize(const vector<int>& minterms, const vector<int>& dontcares){
     //We put all minterms + dontcares together to avoid duplicates
     set<int> terms(minterms.begin(),minterms.end());
     terms.insert(dontcares.begin(),dontcares.end());
     
     //Group
     map<int,vector<string>> groups;
     
     for(int t:terms){
     	string b = toBinary(t); // binary string (length = num_vars)
     	int ones = count(b.begin(),b.end(), '1'); // count how many 1s
     	groups[ones].push_back(b); // group by number of 1s
     }
     
     set<string> prime_implicants; // stores final terms that can't be combined any further
     bool changed;
     // the above changed (bool flag) is to check whether any new combinations are formed in the current pass in the do while loop
     
     do{
     	map<int,vector<string>> new_groups; // will store new combinations for the next iterations.
     	set<string> marked; //keeps track of words that were combined
     	changed = false;
     	
     	for(auto &[i,group1]:groups){
	    if(groups.find(i+1) == groups.end()) continue;
	    for(const string& a: group1){
	    	for(const string& b: groups[i+1]){
		    string res;
		    if(isComb(a,b,res)){
		    	new_groups[i].push_back(res);
		    	marked.insert(a);
		    	marked.insert(b);
		    	changed = true; // set to true to continue looping 
		    }
		}
	    }
	}
	
	//Store unused terms as prime implicants
	for(auto &[i,group]:groups){
	   for(const string& term:group){
	   	if(marked.find(term) == marked.end()){
		     prime_implicants.insert(term);
		}
	   }
	}
	groups = new_groups;
     } while(changed);
     
     //Prime Implicant chart
     map<string,set<int>> chart;
     for(const string& pi:prime_implicants){
     	for(int m: minterms){
     	    if(covers(pi,toBinary(m))){
     	    	chart[pi].insert(m);
     	    }
     	}
     }
    
     //Essential prime implicants
     set<int> covered; // Keeps track of which minterms are already involved
     vector<string> result; // Stores the final list of implicants used in the simplified SOP
     //Continue selecting implicants until all minterms are covered
     while(covered.size() < minterms.size()){
     	map<int,vector<string>> m_to_pis;
     	for(auto& [pi, covered_mins] : chart){
	    for(int m : covered_mins){
	    	if(covered.find(m) == covered.end()) {
		   m_to_pis[m].push_back(pi);
		}
	    }
	}
	
	// Find essential Prime Implicants (EPIs)
	bool found = false;
	for(auto& [m,pis] : m_to_pis){
	  if(pis.size() == 1){
	   string epi = pis[0];
	   result.push_back(epi);
	   for(int c : chart[epi]) covered.insert(c);
	   chart.erase(epi);
	   found = true;
	   break;
	 }
	}
	
	
	// If no essential implicant was found, pick the PI that covers the most uncovered
	// min terms
	
	if(!found){
	   string best_pi;
	   int max_covered = -1;
	   for(auto& [pi, covered_mins] : chart){
	   	int count = 0;
	   	for(int m : covered_mins){
		    if (covered.find(m) == covered.end()) ++count;
		}
		if(count > max_covered){
		   max_covered = count;
		   best_pi = pi;
		}
		
		result.push_back(best_pi);
		for(int c : chart[best_pi]) covered.insert(c);
		chart.erase(best_pi);
	   }
	}
     }  
     return result;   
}

int main(){
    cout << "Enter number of variables (2 to 5): ";
    cin >> num_vars;
    
    if(num_vars < 2 || num_vars > 5){
    	cout << "Invalid number of variables.\n";
    	return 1;
    }    
    
    // Input min terms
    int n;
    cout << "Enter minterms (space separated), end with -1:\n";
    vector<int> minterms;
    while(cin >> n && n != -1) minterms.push_back(n);
    
    //Input Don't care Terms
    cout << "Enter don't care terms (space separated), end with -1:\n";
    vector<int> dontcares;
    while(cin >> n && n != -1) dontcares.push_back(n);
    
    // Simplify using Quine-McCluskey
    vector<string> simplified = minimize(minterms, dontcares);
    
    // Convert to Literal Format and Print
    cout << "\nSimplified SOP Expression:\n";
    for(size_t i = 0; i < simplified.size(); ++i){
    	cout << toLiteral(simplified[i]);
    	if(i != simplified.size() - 1) cout << " + ";
    }
    cout << endl;
    
    return 0;
}


