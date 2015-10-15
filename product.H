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
  Este archivo contiene la definición de la clase Product y una
  estructura que llamada Product_Simulation_Attributes que contiene los
  atributos utilizados en la simulación para una instancia de Product.
*/

# ifndef PRODUCT_H
# define PRODUCT_H

# include "good.H"

# define WORKED_DAYS_IN_A_YEAR 231


/** Representa la especialización Product de la clase Good.
 *
 * Esta clase representa un bien producido por una Unidad Económica registrada
 * en el SIGESIC.
 *
 * @author Alejandro J. Mujica
 */
class Product : public Good
{
  /// Representa la cantidad máxima de producto que se puede producir.
  real production_capacity;

  /// Producción del año declarada en SIGESIC .
  real production;

  /** Sumatoria de la cantidad a producto vendido a empresas que están en la
      red.
   */
  real internal_sales;

  /** Sumatoria de la cantidad a producto vendido a empresas que no están en la
      red.
   */
  real external_sales;

  /// Jornada laboral en horas por día.
  real workday;

  /// Cantidad de personal administrativo por producto declarado en la UE.
  real num_administrative_staff;

  /// Número de empleados por producto declarados en la UE.
  real num_employees;

  /// Precio declarado de venta.
  real price;

  real other_cost; // Estimado


public:
  Product();

  Product(const Product &);

  Product(Product &&);

  const real & get_production_capacity() const;

  void set_production_capacity(const real &);

  const real & get_production() const;

  void set_production(const real &);

  const real & get_internal_sales() const;

  void set_internal_sales(const real &);

  const real & get_external_sales() const;

  void set_external_sales(const real &);

  const real & get_workday() const;

  void set_workday(const real &);

  const real & get_num_administrative_staff() const;

  void set_num_administrative_staff(const real &);

  const real & get_num_employees() const;

  void set_num_employees(const real &);

  const real & get_price() const;

  void set_price(const real &);

  const real & get_other_cost() const;

  void set_other_cost(const real &);

  real get_relationship_manhours() const;

  Product & operator = (const Product &);

  Product & operator = (Product &&);

  std::string to_dot() const;

  Good_Type get_type() const;

  real & production_at(const size_t &);

  real & stock_at(const size_t &);

  real & internal_requested_quantity_at(const size_t &);

  real requested_quantity_at(const size_t &);

  real & internal_sales_at(const size_t &);

  real & price_at(const size_t &);
};

# endif // PRODUCT_H

