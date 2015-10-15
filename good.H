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
  Este archivo contiene la definición de la clase Good y un tipo
  enumerado llamado Good_Type el cual contiene valores para cada tipo de Good.
*/

# ifndef GOOD_H
# define GOOD_H

# include "types.H"



/// Tipo de especialización de un bien.
enum Good_Type { PRODUCT, INPUT, IMPORTED_PRODUCT };

/** Representa la abstracción base para un Bien.
 *
 *  Esta clase contiene información general que llevan todos los bienes, bien
 *  sea el producto de una empresa registrada en el SIGESIC, un insumo declarado
 *  por una empresa registrada el cual sea producido por una empresa no
 *  registrada o una importación declarada por una empresa registrada.
 *
 * @author Alejandro J. Mujica
 */
class Good
{
  // Información de la empresa que lo produce

  /// Registro de Información Fiscal de la Unidad Económica.
  std::string company_rif;

  /// Razón Social.
  std::string company_name;

  /// Ubicación
  std::string company_location;

  /// Nacionalidad.
  std::string nationality;

  // Información general del bien

  /** Id del bien en la tabla de la base de datos, tanto para producto como para
   *  insumo.
   */
  db_id_t     id;
  std::string name;
  std::string tariff_code;
  std::string measurement_unit;

  // Información del bien en la red
  int level;

public:
  Good();

  Good(const Good &);

  Good(Good &&);

  const std::string & get_company_rif() const;

  void set_company_rif(const std::string &);

  void set_company_rif(std::string &&);

  const std::string & get_company_name() const;

  void set_company_name(const std::string &);

  void set_company_name(std::string &&);

  const std::string & get_company_location() const;

  void set_company_location(const std::string &);

  void set_company_location(std::string &&);

  const std::string & get_nationality() const;

  void set_nationality(const std::string &);

  void set_nationality(std::string &&);

  const db_id_t & get_id() const;

  void set_id(const db_id_t &);

  const std::string & get_name() const;

  void set_name(const std::string &);

  void set_name(std::string &&);

  const std::string & get_tariff_code() const;

  void set_tariff_code(const std::string &);

  void set_tariff_code(std::string &&);

  const std::string & get_measurement_unit() const;

  void set_measurement_unit(const std::string &);

  void set_measurement_unit(std::string &&);

  const int & get_level() const;

  void set_level(const int &);

  Good & operator = (const Good &);

  Good & operator = (Good &&);

  virtual std::string to_dot() const;

  virtual Good_Type get_type() const = 0;

  virtual real & production_at(const size_t &) = 0;

  virtual real & stock_at(const size_t &) = 0;

  virtual real & internal_requested_quantity_at(const size_t &) = 0;

  virtual real requested_quantity_at(const size_t &) = 0;

  virtual real & internal_sales_at(const size_t &) = 0;

  virtual real & price_at(const size_t &) = 0;
};

# endif // GOOD_H

