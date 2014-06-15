// Projekt2C.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

/*** Ispitivanje da li vector listaGradova sadrži string cityName ***/
bool containsCity(string cityName, vector<string> listaGradova){
	for(int i=0; i<listaGradova.size(); i++){
		if(listaGradova[i] == cityName)
		{
			return true;
		}
	}
		return false;
}

/*** Vraća redni broj grada iz vector-a listaGradova koji je string cityName ***/
int getNoCity(string cityName, vector<string> listaGradova){
	for(int i=0; i<listaGradova.size(); i++){
		if(listaGradova[i] == cityName)
		{
			return i;
		}
	}
		return -1;
}

/*** Ispisuju se svi gradovi s kojim je neki grad povezan - direktno ili indirektno ***/
void pFunc(string city, vector<string> imenaGradova, vector< vector<double> > floyd){
	int n = getNoCity(city, imenaGradova);
	for(int i = 0; i < imenaGradova.size(); i++)
	{
		if(floyd[n][i] != DBL_MAX && floyd[n][i] != 0)
		{
			cout << imenaGradova[i] << " " << floyd[n][i] << endl;
		}
	}
}

/*** Ispisuje se najkraći put između dva grada ***/
void nFunc(string city1, string city2, vector<string> imenaGradova, vector< vector<double> > floyd){
	cout << city1 << " - " << city2 << " : ";
	int n = getNoCity(city1, imenaGradova);
	int m = getNoCity(city2, imenaGradova);
	cout << floyd[m][n] << endl;
}

/*** Ispisuje se koliko kilometara je najviše, a koliko najmanje potrebno rekonstruirati da bi se spojili svi gradovi s kojima je početni grad prometno povezan ***/
void rFunc(string city, vector<string> imenaGradova, vector< vector<double> > floyd, vector< vector<int> > path){
	int n = getNoCity(city, imenaGradova), m;
	vector<string> sviGradovi;
	for(int i = 0; i < imenaGradova.size(); i++){
		if( path[n][i] != INT_MAX) sviGradovi.push_back(imenaGradova[i]);
	}
	double min = 0, max = 0;
	for (int i=0; i<imenaGradova.size();i++){
		if(containsCity(imenaGradova[i], sviGradovi)){
			for( int j = i +1; j<imenaGradova.size(); j++){
				if(containsCity(imenaGradova[j], sviGradovi)){
					max += floyd[i][j];
				}
			}
		}
	}
	for (int i=0; i<imenaGradova.size();i++){
		if(containsCity(imenaGradova[i], sviGradovi)){
			n = getNoCity(city, imenaGradova);
			m = i;
			min += floyd[n][m];
			while(n != m){
				n = path[n][m];
				if(containsCity(imenaGradova[n], sviGradovi))
					sviGradovi.erase(sviGradovi.begin()+getNoCity(imenaGradova[n], sviGradovi));
			}
		}
	}
	cout << "Min: " << min << " Max: " << max << endl;
}

int _tmain(int argc, char *argv[])
{
	clock_t t1, t2;
	t1=clock();

	string line, cityName1, cityName2;
	double distance;
	int i,j;
	vector<string> imenaGradova;

	/*** ifstream init argv[1] -> datoteka ***/
	ifstream is ( argv[1] );
	if(!is){
		return 0;
	}
	/*** Učitavanje gradova ***/
	while(!is.eof()){
		is >> line;
		if(!containsCity(line, imenaGradova)) imenaGradova.push_back(line);
		is >> line;
		if(!containsCity(line, imenaGradova)) imenaGradova.push_back(line);
		is >> line;
	}
	/*** matrix -> težinska matrica ***/ 
	/*** path -> Floyd–Warshall matrica za rekonstrukciju puta **/
	vector<vector<double>> matrix (imenaGradova.size(), vector<double>(imenaGradova.size(), DBL_MAX));
	vector<vector<int>> path(imenaGradova.size(), vector<int>(imenaGradova.size(), INT_MAX));
	
	is.clear();
	is.seekg(0, ios::beg);

	/*** Unos težina ***/
	while(!is.eof()){
		is >> cityName1;
		is >> cityName2;
		is >> distance;
		i = getNoCity(cityName1, imenaGradova);
		j = getNoCity(cityName2, imenaGradova);
		matrix[i][j] = distance;
		matrix[j][i] = distance;
		path[i][j] = j;
		path[j][i] = i;
	}

	/*** Postavljanje dijagonale u 0 ***/
	for(int i = 0; i <imenaGradova.size(); i++){
		matrix[i][i] = 0;
	}

	/*** Floyd–Warshall računanje min puta ***/
	vector<vector<double>> floyd = matrix;

	/*** Implementacija Floyd–Warshall algoritma ***/
	for(int k = 0; k < imenaGradova.size(); k++)
		for(int i = 0; i < imenaGradova.size(); i++)
			for(int j = 0; j < imenaGradova.size(); j++)
                if(floyd[i][j]>floyd[i][k]+floyd[k][j]){
                    floyd[i][j]=floyd[i][k]+floyd[k][j];
					path[i][j] = path[i][k];
				}

	/*** Poziv opcija ***/
	for(int i=2; i<argc; i++){
		if(strcmp(argv[i], "-p")== 0){
			pFunc(argv[i+1], imenaGradova, floyd);
		}else if(strcmp(argv[i],"-n")==0){
			nFunc(argv[i+1], argv[i+2], imenaGradova, floyd);
		}else if(strcmp(argv[2],"-r")==0){
			rFunc(argv[i+1], imenaGradova, floyd, path);
		}
    }

	t2 = clock();
	float sekunde=((float)t2-(float)t1/CLOCKS_PER_SEC);
	cout << sekunde << endl;

	system("pause");
	return 0;
}

