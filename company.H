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
  Fecha de creación: 09/06/2014
  Este archivo contiene la definición de la abstracción de empresa o unidad
  económica.
*/

# ifndef COMPANY_H
# define COMPANY_H

# include <types.H>

/** Clase que representa una empresa o unidad económica dentro de la red
 *  productiva.
 *
 * Esta clase permite abstraer el concepto de una empresa, pues en una red
 * productiva pueden existir varios productos producidos por la misma empresa
 * en esos casos la existencia el concepto de la empresa es uno solo y al 
 * bien se le asigna una referencia a éste.
 *
 * @author Alejandro J. Mujica
 */
class Company
{
  /// Registro de Información Fiscal de la Unidad Económica.
  std::string rif;

  /// Razón Social.
  std::string name;

  /** Jornada de trabajo diaria (Se usa como real porque las operaciones sobre,
   *  este atributo pueden dar números fraccionario).
   */
  real working_day;

public:
  Company();

  Company(const Company &);

  Company(Company &&);

  const std::string & get_rif() const;

  void set_rif(const std::string &);

  void set_rif(std::string &&);

  const std::string & get_name() const;

  void set_name(const std::string &);

  void set_name(std::string &&);

  const real & get_working_day() const;

  void set_working_day(const real &);

  Company & operator = (const Company &);

  Company & operator = (Company &&);
};

# endif // COMPANY_H

