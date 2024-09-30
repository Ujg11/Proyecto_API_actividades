# Proyecto_API_actividades

## Introducció
Prova tècnica per assistir al Hackató del Saló de l'Ocupació.

Es tracta d'un servidor API que gestiona usuaris i activitats. Permet realitzar operacions de creació, lectura, actualització i eliminació (CRUD) sobre les dades dels usuaris i creació, lectura i eliminació (CRD) sobre les activitats. També permet exportar activitats en format JSON i importar activitats des d'un arxiu JSON.
A part, crea i estableix connexió amb una base de dades de MySQL per guardar els usuaris i les activitats i els enllaços entre ells.

El projecte està implementat en C++ i utilitza les llibreries mysql-connector-cpp per a la gestió de la base de dades MySQL i json.hpp per a la manipulació de dades en format JSON.


## Instal·lació

### 1- Clonar el repositori:
```bash
git clone <url_del_repositori>
```

### 2- Compilar el projecte:
S'utilitza Makefile. Per compilar el projecte, simplement cal posar `make` a la terminal i es crearà el binari `api`.Per borrar els arxius objecte `.o`,executa:
```bash
make clean
```

Per últim, per borrar els arxius objecte i també el binari, executa:
```bash
make fclean
```

### 3- Executar el projecte:
Per executar el projecte cal fer el següent:
```bash
./api <sql_user> <sql_password>
```

S'ha d'executar amb un usuari que tingui permís de MySQL per poder crear la taula de dades.

### 4- Funcionament:
Alexecutar es podra veure que el servidor esta escoltant al port 8080 i espera a rebre conexions amb la informació adeqüada. Per probar el projecte desde la terminal podem fer el següent:

###### Crear Usuari:
```bash
curl -X POST http://localhost:8080/appActivitats/user -H "Content-Type: application/json" -d '{"nom": "Juan", "cognoms": "Pérez", "dni": "46099155W" , "edat": 30, "email": "juan.perez@example.com"}'
```

###### Actualitzar Usuari:
```bash
curl -X PUT http://localhost:8080/appActivitats/users/1 \ -H "Content-Type: application/json" \ -d '{"nom": "Juan", "cognoms": "Pérez", "dni": "12345678A", "edat": 31, "email": "juan.perez@example.com"}' 
```

###### Obtenir Usuari:
```bash
curl -X GET http://localhost:8080/appActivitats/users/1
```

###### Eliminar Usuari:
```bash
curl -X DELETE http://localhost:8080/appActivitats/users/1
```

###### Crear Activitat:
```bash
curl -X POST http://localhost:8080/appActivitats/activitats \
-H "Content-Type: application/json" \
-d '{"nom": "Futbol", "descripcio": "Partit de futbol 11 contra 11", "capacitat_maxima": 22}'
```

###### Obtenir Activitat:
```bash
curl -X GET http://localhost:8080/appActivitats/activitats/1
```

###### Eliminar Activitat:
```bash
curl -X DELETE http://localhost:8080/appActivitats/activitats/1
```

###### Apuntar Usuari a Activitat: (id usuari, id activitat)
```bash
curl -X POST http://localhost:8080/appActivitats/activitats/1/apuntar/1
```

###### Desapuntar Usuari de l’Activitat: (id usuari, id activitat)
```bash
curl -X POST http://localhost:8080/appActivitats/activitats/1/desapuntar/1
```

###### Exportar Activitats:
```bash
curl -X GET http://localhost:8080/appActivitats/activitats/exportar
```

###### Importar Activitats:
```bash
curl -X POST http://localhost:8080/appActivitats/activitats/importar \
-H "Content-Type: application/json" \
-d '[{"nom": "Tennis", "descripcio": "Partit de tennis entre dos", "capacitat_maxima": 2}]'
```

