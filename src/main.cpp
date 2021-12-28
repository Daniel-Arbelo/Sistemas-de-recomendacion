#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <math.h>

using namespace std;

// Si la cadena se encuentra en el vector de cadenas, devuelve true, si no false
bool isin(string cadena, vector<string> stopwords){
    vector<int>::size_type sz = stopwords.size();
    for(unsigned int i = 0; i< sz; i++) {
        if(cadena.compare(stopwords[i]) == 0)
            return 1;
    }
    return 0;
}

// Si la cadena se encuentra en el tf, se devuelve la pos donde se encuentra, si no se devuelve -1
int posCadena(vector<pair<string, auto>> tf, string cadena){
    vector<int>::size_type sz = tf.size();
    for(unsigned int i = 0; i< sz; i++) {
        if(cadena.compare(tf[i].first) == 0)
            return i;
    }
    return -1;
}

// Se le pasa el nombre del documento y se crea el TF
vector<pair<string, int>> crearTF(string nombreDoc, vector<string> stopWords){
    vector<pair<string, int>> tf;
    string cadena;
    int pos;

    ifstream fe(nombreDoc);

    while (!fe.eof()) {
        fe >> cadena;
        // Eliminación de los signos de puntuacion
        if(cadena.back() == '.' || cadena.back() == ','){
            cadena.erase(cadena.end()-1);
        }
        // Eliminación de palabras (stop-words)
        if(!isin(cadena, stopWords)){
            pos = posCadena(tf, cadena); // si la cadena existe en el vector devuelve la pos, si no devuelve -1
            if(pos != -1){
                tf[pos].second ++;
            }else{
                tf.push_back(pair(cadena, 1));
            }
        }
            
    }
    fe.close();
    return tf;
}

double IDF(string palabra, vector<vector<pair<string, int>>> tfs){
    double numeroDocs = tfs.size();
    double vecesqueserepite = 0;
    double sol;

    for(int i = 0; i < numeroDocs; i++){
        if(posCadena(tfs[i], palabra) != -1){
            vecesqueserepite ++;
        }
    }
    //cout << palabra << numeroDocs << vecesqueserepite << numeroDocs/vecesqueserepite << endl;
    if(vecesqueserepite == 0)
      return 0;
    double division = numeroDocs/vecesqueserepite;
    sol = log10(division);
    return sol;
}

vector<vector<pair<string, double>>> crearIDFs(vector<vector<pair<string, int>>> tfs){
    vector<pair<string, double>> idf;
    vector<vector<pair<string, double>>> IDFs;
    double valor;

    vector<int>::size_type sz1 = tfs.size();
    vector<int>::size_type sz2;
    for(unsigned int i = 0; i < sz1; i++) {
        sz2 = tfs[i].size();
        for(unsigned int j = 0; j < sz2; j++){
            valor = IDF(tfs[i][j].first, tfs);
            idf.push_back(pair(tfs[i][j].first, valor));
        }
        IDFs.push_back(idf);
        idf.erase(idf.begin(), idf.end());
    }
    
    return IDFs;
}

vector<vector<pair<string, double>>> crearTF_IDFs(vector<vector<pair<string, int>>> tfs, vector<vector<pair<string, double>>> IDFs){
    vector<pair<string, double>> tf_IDF;
    vector<vector<pair<string, double>>> tf_IDFs;
    
    double valortf, valor;
    vector<int>::size_type sz1 = tfs.size();
    vector<int>::size_type sz2;
    for(unsigned int i = 0; i < sz1; i++){
        sz2 = tfs[i].size();
        for(unsigned int j = 0; j < sz2; j++){
            valortf = tfs[i][j].second;
            valor = valortf * IDFs[i][j].second;
            //cout << valortf << " * " <<  IDFs[i][j].second << " = " << valor << endl;
            tf_IDF.push_back(pair(tfs[i][j].first, valor));
        } 
        tf_IDFs.push_back(tf_IDF);
        tf_IDF.erase(tf_IDF.begin(), tf_IDF.end());
    }

    return tf_IDFs;
}

double cos(vector<pair<string, double>> art1, vector<pair<string, double>> art2){
    int poscadena;
    double solucion = 0;

    vector<int>::size_type sz = art1.size();
    for(unsigned int i = 0; i < sz; i++){
        if(posCadena(art2, art1[i].first) != -1){
            poscadena = posCadena(art2, art1[i].first);
            solucion +=  art1[i].second * art2[poscadena].second;
        }
    }
    return solucion;
}

void imprimir(vector<vector<pair<string, auto>>> aimprimir){
    vector<int>::size_type sz1;
    vector<int>::size_type sz = aimprimir.size();
    cout << sz << endl;
    for(unsigned int i = 0; i< sz; i++){ // recorre los documentos
        sz1 = aimprimir[i].size();
        cout << "//////////Documento " << i+1 << " //////////" << endl;
        for(unsigned int j = 0; j< sz1; j++){
            cout << "(" << aimprimir[i][j].first << ", " << aimprimir[i][j].second << ")" << endl;
        }
    }
}

int main(int argc, char *argv[]) {
    string cadena;
    vector<vector<pair<string, int>>> tfs;
    vector<vector<pair<string, double>>> IDFs;
    vector<vector<pair<string, double>>> tf_IDFs;
    vector<pair<string, int>> tf;
    
    // Leer las palabras que se van a descartar del fichero
    vector<string> stopWords;
    
    ifstream fesw("stop_words.txt");

    while (!fesw.eof()) {
        fesw >> cadena;
        stopWords.push_back(cadena);
        
    }
    fesw.close();

    // Se leen los documentos y se crean los TFS
    int numeroDocs; // La primera linea contiene el numero de documetnos
    ifstream docs(argv[1]);
    docs >> numeroDocs;
    for(int i = 0; i < numeroDocs; i++) {
        docs >> cadena;
        cout << cadena << endl;
        tf = crearTF(cadena, stopWords);
        tfs.push_back(tf);
        tf.erase(tf.begin(), tf.end());
    }
    docs.close();

    // Calcular el IDF
    IDFs = crearIDFs(tfs);

    // Calcular TF-IDF
    tf_IDFs = crearTF_IDFs(tfs, IDFs);

    // Imprimir tfs
    cout << "TFS" << endl;
    imprimir(tfs);
    cout << "IDF" << endl;
    imprimir(IDFs);
    cout << "TF - IDF" << endl;
    imprimir(tf_IDFs);
    
    // Hallar la similaridad entre los artículos
    cout << "cos(Articulo1.txt, Articulo2.txt) = " << cos(tf_IDFs[0], tf_IDFs[1]) << endl;

    
    
}