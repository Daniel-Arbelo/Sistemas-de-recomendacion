# Sistemas de recomendación. Modelos Basados en el Contenido


_El objetivo de esta práctica es implementar un sistema de recomendación siguiendo el modelo basados en el contenido._


### Pre-requisitos 📋

_Tener instalado el makefile y el compilador g++. Se puede instalar:_

```
install ubuntu-make
sudo apt-get update
sudo apt-get install g++
```

### Compilación 🔧

_La copilación es bastante sencilla, ya que con el makefile solo habria que po ner la linea siguiente en la terminal_


```
make
```

_Y cada vez que se cambia algo en el código hacer lo mismo antes de volver a ejecutar_


## Ejecución ⚙️

_ Para ejecutar el programa se le especifica por comandos el fichero  que contiene el nombre de los documentos que se van a analizar, que en este caso es el fichero docs.txt que contiene el nombre de los documentos. El formato es: En la primera línea el número de documentos que hay y en las siguientes líneas los documentos 1, 2 y 3. Para ejecutar el programa sería:_

...
./bin/main docs.txt
...

### Codificación ⌨️

_Los valores de TF IDF y TF-IDF se guardan en vectores de vectores de pares de string, (int para los tfs y double para el resto). La primera indexación es para acceder al documento, el segundo para el término, que se almacena en lel string del pair, y en el int del pair se almacena el valor de ese término, dependiendo si se trata del tf, idf ó tf-idf_

```
vector<vector<pair<string, int>>> tfs;
vector<vector<pair<string, double>>> IDFs;
vector<vector<pair<string, double>>> tf_IDFs;
```
_Para inicializar esto hay que en primer lugar leer los el documento que contiene los documentos, y en primer lugar se inicializan los tfs, pero antes leemos el fichero que contiene las stopwors y las guardamos en un vector de strings, que van a ser las palabras que se van a descartar. Para inicializar los tfs, se le pasa a lafunción crearTF el nombre del documento y el vetor con las stopwords_
...
vector<pair<string, int>> crearTF(string nombreDoc, vector<string> stopWords){
...
_Esta función lo que hace es recorrer el documento, descartando las stopwords y mirando si ya se había leido el término, si es así se le suma uno a las veces que se repite ese término, si no se añade al vector de pares que se devolverá al recorrer todo el documento.
Después de esto se inicializan los IDFs que se hace mediante la función:_
...
vector<vector<pair<string, double>>> crearIDFs(vector<vector<pair<string, int>>> tfs){ 
...
_Que recorre el los vectores y para cada término llama a la función IDF que devuelve el valor IDF viendo las veces que se repite el término en los documentos y lo divide entre el numero de documentos, para luego hacer el log de esto. 
Por último para inicializar los TF - IDFs es bastante sencillo, mediante la función:_
...
vector<vector<pair<string, double>>> crearTF_IDFs(vector<vector<pair<string, int>>> tfs, vector<vector<pair<string, double>>> IDFs)
...
_Se recorren los TFs y los IDFs y se multiplican el uno por el otro._


