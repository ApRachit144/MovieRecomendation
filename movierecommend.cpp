#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <cmath>
#include <utility>
#include <algorithm>
#include <stdexcept>
using namespace std;


vector<pair<int,int>> sortMovies(vector<vector<int>>& matrix, int user) {
    if (user < 0 || user >= matrix.size()) {
        cout << "Invalid row index." << endl;
        return {};
    }

   

    vector<int> sort;
    vector<int>& row = matrix[user];
      vector<int> indx;
      vector<int> copy;
    for(int i = 0 ; i < row.size() ; i++){
        copy.push_back(row[i]);
    }
   
  for(int i = 0 ; i< row.size();i++){
            int max = row[i];
        for(int j = i+1 ; j<5;j++){
                if(max<row[j]){
                    int a = row[j];
                    row[j] = max;
                    max = a;
                }
        }
                sort.push_back(max);
    }


    for(int i = 0 ; i < sort.size(); i++){
            for(int j = 0 ; j < 5 ; j++){
            if(sort[i] == copy[j]){
                copy[j] = 0;
                indx.push_back(j+1);
                
            }
            }
    }
     vector<pair<int,int>> p;
  for(int i = 0 ;  i<sort.size();i++){
    p.push_back({indx[i],sort[i]});
  }
   return p;
}



bool compareter(pair<int,int> a , pair<int,int> b){

      return a.second > b.second;

}
vector<vector<int>> readDataFromCsv(string &filename) {
    vector<vector<int>> ratingmatrix;
    ifstream csvfile(filename);
    if (!csvfile.is_open()) {
        cout<<"There is an error. File can't be open";
        return ratingmatrix;
    }

    string line;
    while (getline(csvfile, line)) {
        vector<int> ratingofOne;
        string stars;
        stringstream ss(line);
        while (getline(ss, stars, ',')) {
            try {
                ratingofOne.push_back(stoi(stars));
            } catch (exception &e) {
                cerr << "Invalid data in CSV: " << stars << endl;
                ratingofOne.push_back(0);
            }
        }
        ratingmatrix.push_back(ratingofOne);
    }
    csvfile.close();
    return ratingmatrix;
}


double Similarityofuser(vector<int> &user1, vector<int> &user2) {
    int dotProduct = 0, mangA = 0, mangB = 0;
    for (int i = 0; i < user1.size(); i++) {
        dotProduct += user1[i] * user2[i];
        mangA += user1[i] * user1[i];
        mangB += user2[i] * user2[i];
    }
    if (mangA == 0 || mangB == 0) {
        return 0.0; 
    }
    return dotProduct / (sqrt(mangA) * sqrt(mangB));
}

double predictrating(vector<vector<int>> &matrix, int UserIndx, int movieIndx) {
    double TotalSimilarity = 0;
    double TotalSimilarityWithRate = 0;

    for (int i = 0; i < matrix.size(); i++) {
        if (i != UserIndx && matrix[i][movieIndx] != 0) {
            double Similarity = Similarityofuser(matrix[UserIndx], matrix[i]);
            TotalSimilarity += Similarity;
            TotalSimilarityWithRate += Similarity * matrix[i][movieIndx];
        }
    }
    if(TotalSimilarity!=0){
    return (TotalSimilarityWithRate / TotalSimilarity);
    }
return 0;

}

vector<pair<int, double>> recommendMovies(vector<vector<int>> &matrix, int userIndx, int topmovie) {
    vector<pair<int, double>> predictions;
    for (int movieIndx = 0; movieIndx < matrix[userIndx].size(); ++movieIndx) {
        if (matrix[userIndx][movieIndx] == 0) {
            double predictedRating = predictrating(matrix, userIndx, movieIndx);
            if (predictedRating > 0) { 
                predictions.push_back({movieIndx, predictedRating});
          
            }
        }
    }

    sort(predictions.begin(), predictions.end(),compareter);


    return predictions;
}

int main() {
    string filename = "D:/MovieRecomendation/ratings.csv";
   
        vector<vector<int>> matrix = readDataFromCsv(filename);

        int user;
        int topMovie = 4; 

        cout << "Enter a user from (1 to " << matrix.size()<<")"<<" to which you want recomendation : ";
        cin >> user;

        if (user < 1 || user > matrix.size()) {
            cerr << "Invalid user index." << endl;
            return 1;
        }

        vector<pair<int, double>> r = recommendMovies(matrix, user - 1, topMovie);
        
      
      for(const auto &recom: r){
        for(int i = 0 ; i< matrix[(user-1)].size();i++){
            if(matrix[(user-1)][i]==0){
                matrix[(user-1)][i] = recom.second;
                break;
            }
        }
      }
        
     vector<pair<int,int>> movieno = sortMovies(matrix, (user-1));
cout<<"Here is the top 5 recomended movies for you !!!"<<endl;
for(int i = 0 ; i<movieno.size();i++){
    cout<<"Movie : "<< movieno[i].first << " with rating "<< movieno[i].second<<endl;
}



    return 0;
    }

    //This is the code for movie recomendation and taking input from csv file
    