/*
  Copyright (C) 2014
  Alejandro Mujica (amujica@cenditel.gob.ve)
  José Ruiz (jruiz@cenditel.gob.ve)
  Julie Vera (jvera@cenditel.gob.ve)
 
  CENDITEL Fundación Centro Nacional de Desarrollo e Investigación en
  Tecnologías Libres
 
  Este programa es software libre; Usted puede usarlo bajo los términos de la
  licencia de software GPL versión 2.0 de la Free Software Foundation.
 
  Este programa se distribuye con la esperanza de que sea útil, pero SIN
  NINGUNA GARANTÍA; tampoco las implícitas garantías de MERCANTILIDAD o
  ADECUACIÓN A UN PROPÓSITO PARTICULAR.
  Consulte la licencia GPL para más detalles. Usted debe recibir una copia
  de la GPL junto con este programa; si no, escriba a la Free Software
  Foundation Inc. 51 Franklin Street,5 Piso, Boston, MA 02110-1301, USA.
*/

/*
  Autor:             Alejandro J. Mujica
  Fecha de creación: 05/06/2014
  Este archivo contiene la definición de nombres para tipos de datos utilizados.
*/

# ifndef TYPES_H
# define TYPES_H

# include "includes.H"

/// Define la precisión en punto flotante a utilizar
typedef double real;

/// Define el tipo numérico para los id de la base de datos
typedef unsigned long long db_id_t;

/// Define el tipo de arreglo a utilizar.
# define Array DynArray

/// Define el tipo de lista a utilizar.
# define List DynDlist

/// Define el tipo de cola a utilizar.
# define Queue DynListQueue

/// Define el tipo de conjunto a utilizar.
# define Set DynSetTreap

/// Define el tipo de mapa a utilizar.
# define Map DynMapTreap

# endif // TYPES_H

