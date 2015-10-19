# ifndef FETCHDB_H
# define FETCHDB_H

#include <stdio.h>
#include <iostream>
#include <dbQuery.H>
#include <string>
#include <fstream>
#include <locale>

class Fetch{

	
	private:

	DBConnection connection;
	DBQuery *q = new DBQuery(connection);
	std::string query;
	int cont;
	

	public:

	Fetch();

	~Fetch(){}

	Fetch(std::string);

	void unidades_economicas(std::string);
	void subUnidadesEconomicas(std::string);
	void produccion_producto(std::string);
	void produccion_insumo(std::string);
	void proveedores_insumo(std::string);
        void proveedores_proveedor(std::string);
	void producto_t_insumo(std::string);
	void socios(std::string);
	void aditionalUE(std::string);
	

	};

Fetch::Fetch() {

	query=" ";
	cont=0;
}

Fetch::Fetch(const std::string name)
{
	connection.open(name.c_str());
	cont=0;
	
	std::ofstream file;
	file.open("unidad_economica.csv");
	file<<"rif,nombre"<<std::endl;
	file.close();

	file.open("unidadecon_subunidad_economica.csv");
	file<<"id,ue_rif,nombre"<<std::endl;
	file.close();

	file.open("produccion_producto.csv");
	file<<"id,nombre,codigo_aran_id,planta_id_id"<<std::endl;
	file.close();

	file.open("produccion_insumo.csv");
	file<<"id,nombre,codigo_aran_id,planta_id_id"<<std::endl;
	file.close();

	file.open("cmproveedores_proveedorinsumo.csv");
	file<<"id,proveedor_id_id,insumo_id_id,comprado_r,cantidad_comprada_r"<<std::endl;
	file.close();
	
	file.open("cmproveedores_proveedor.csv");
	file<<"id,nombre,rif,pais_origen_id,pais_procedencia_id,nacional,planta_id_id"<<std::endl;
	file.close();

	file.open("produccion_producto_t_insumo.csv");
	file<<"id,insumo_id_id,producto_id_id,anho,cantidad,activo,fecha_insercion"<<std::endl;
	file.close();

	file.open("unidadecon_socio.csv");
	file<<"blank,id, unidad_economica_rif, rif_pasaporte, nombre, nacionalidad,tipo_persona_id, porcentaje"<<std::endl;	
	file.close();

}

void Fetch::unidades_economicas(std::string rif)
{


        std::locale loc;
   	std::size_t found;
	std::string nombre,query;
	rif[0]=std::toupper(rif[0],loc);

	
	subUnidadesEconomicas(rif);

	socios(rif);

	query=(std::string)"SELECT rif, nombre FROM unidadecon_unidad_economica WHERE rif="+"\'"+rif+"\'";
	
	std::ofstream file;

	file.open("unidad_economica.csv", std::ios::app);

	q->exec(query);
	q->reset();

	while(q->hasResult() && q->next())
	{
	 rif=q->getValue(0);
	 nombre=q->getValue(1);

         
	found=nombre.find("\"");

	while(found!=std::string::npos)
	{

	 nombre.erase(found,1);
	 found=nombre.find("\"");
	
	}
	
	file<<rif<<",\""<<nombre<<"\""<<std::endl;	
	}
	q->clear();
	file.close();

}


void Fetch::subUnidadesEconomicas(std::string search)
{

	
	int aux;
	std::size_t found;
	std::string ue_rif, nombre, ident,id ;
	
	std::ofstream file;

	file.open("unidadecon_subunidad_economica.csv", std::ios::app);

	
	query=(std::string)"SELECT id, ue_rif, nombre FROM unidadecon_subunidad_economica WHERE ue_rif="+"\'"+search+"\'";
	
	q->exec(query);
	q->reset();


	if(q->hasResult())
	{
	
		while(q->hasResult() && q->next())
		{

		 ident=q->getValue(0);
		 aux=atoi(ident.c_str());
		 if(cont==0)
		 aux=aux+1;
		 cont++;
		 id=std::to_string(aux);
          	 ue_rif=q->getValue(1);
		 nombre=q->getValue(2);
        
 		 found=nombre.find("\"");

			while(found!=std::string::npos)
			{
	
			 nombre.erase(found,1);
			 found=nombre.find("\"");
		
			}
	
		
		
		produccion_producto(id);
		proveedores_proveedor(id);

		std::cout<<id<<","<<ue_rif<<",\""<<nombre<<"\""<<std::endl;	
		file<<id<<","<<ue_rif<<",\""<<nombre<<"\""<<std::endl;	
		}

	}

	else
	file<<"00"<<","<<"0000000000"<<",\""<<"NO ENCONTRADO"<<"\""<<std::endl;
	

	q->clear();
	file.close();

}

void Fetch::produccion_producto(std::string planta)
{

	std::size_t found;
	std::string id,nombre,codigo_aran_id,planta_id_id;
	
	std::ofstream file;

	file.open("produccion_producto.csv",std::ios::app);

	query=(std::string)"SELECT id, nombre, codigo_aran_id, planta_id_id FROM produccion_producto WHERE planta_id_id="+"\'"+planta+"\'";

	q->exec(query);
	q->reset();

	if(q->hasResult())
	{

		while(q->hasResult() && q->next())
		{
	
		 id=q->getValue(0);
		 nombre=q->getValue(1);
		 codigo_aran_id=q->getValue(2);
		 planta_id_id=q->getValue(3);
        	
 		 found=nombre.find("\"");
	
			while(found!=std::string::npos)
			{
		
			 nombre.erase(found,1);
			 found=nombre.find("\"");
			
			}
		
		producto_t_insumo(id);
		file<<id<<",\""<<nombre<<"\","<<codigo_aran_id<<","<<planta_id_id<<std::endl;	

		}
	}

	else
	file<<"00"<<",\""<<"NO ENCONTRADO"<<"\","<<"0000000000"<<","<<"0000000000"<<std::endl;

	q->clear();
	file.close();

}

void Fetch::produccion_insumo(std::string insumo)
{
	std::size_t found;
	std::string id,nombre,codigo_aran_id,planta_id_id;
	
	std::ofstream file;

	file.open("produccion_insumo.csv",std::ios::app);

	query=(std::string)"SELECT id, nombre, codigo_aran_id, planta_id_id FROM produccion_insumo WHERE id="+"\'"+insumo+"\'";

	q->exec(query);
	q->reset();

	if(q->hasResult())
	{

		while(q->hasResult() && q->next())
		{
	
		 id=q->getValue(0);
		 nombre=q->getValue(1);
		 codigo_aran_id=q->getValue(2);
		 planta_id_id=q->getValue(3);
        	
 		 found=nombre.find("\"");
	
			while(found!=std::string::npos)
			{
		
			 nombre.erase(found,1);
			 found=nombre.find("\"");
			
			}
		
		
		aditionalUE(planta_id_id);
	
		

		file<<id<<",\""<<nombre<<"\","<<codigo_aran_id<<","<<planta_id_id<<std::endl;	

		}
	}

	else
	file<<"00"<<",\""<<"NO ENCONTRADO"<<"\","<<"0000000000"<<","<<"0000000000"<<std::endl;

	q->clear();
	file.close();




}

void Fetch::proveedores_insumo(std::string insumo)
{

	//std::size_t found;
	std::string id,proveedor_id_id,insumo_id_id,comprado_r,cantidad_comprada_r,query;
	
	std::ofstream file;

	file.open("cmproveedores_proveedorinsumo.csv", std::ios::app);

	query=(std::string)"SELECT id,proveedor_id_id,insumo_id_id,comprado_r,cantidad_comprada_r FROM cmproveedores_proveedorinsumo WHERE insumo_id_id="+"\'"+insumo+"\'";

	q->exec(query);
	q->reset();

	if(q->hasResult())
	{	
		while(q->hasResult() && q->next())
		{
	
		 id=q->getValue(0);
		 proveedor_id_id=q->getValue(1);
		 insumo_id_id=q->getValue(2);
		 comprado_r=q->getValue(3);
		 cantidad_comprada_r=q->getValue(4);
        	
 		file<<id<<",\""<<proveedor_id_id<<"\","<<insumo_id_id<<","<<comprado_r<<","<<cantidad_comprada_r<<std::endl;	
		}

	}

	else
	
	file<<"00"<<",\""<<"00000"<<"\","<<"00000"<<","<<"NONE"<<","<<"NONE"<<std::endl;	


	q->clear();
	file.close();

}

void Fetch::proveedores_proveedor(std::string planta)
{

	std::size_t found;
	std::string id,nombre,rif,pais_origen_id,pais_procedencia_id,nacional,planta_id_id,query;
	
	std::ofstream file;

	file.open("cmproveedores_proveedor.csv", std::ios::app);

	query=(std::string)"SELECT id,nombre,rif,pais_origen_id,pais_procedencia_id,nacional,planta_id_id FROM cmproveedores_proveedor WHERE planta_id_id="+"\'"+planta+"\'";

	q->exec(query);
	q->reset();

	if(q->hasResult())
	{	

		while(q->hasResult() && q->next())
		{
	
		 id=q->getValue(0);
		 nombre=q->getValue(1);
		 rif=q->getValue(2);
		 pais_origen_id=q->getValue(3);
		 pais_procedencia_id=q->getValue(4);
		 nacional=q->getValue(5);
 		 planta_id_id=q->getValue(6);
        	
		 found=nombre.find("\"");
	
			while(found!=std::string::npos)
			{
		
			 nombre.erase(found,1);
			 found=nombre.find("\"");
			
			}
	
		proveedores_insumo(id);
		
		file<<id<<",\""<<nombre<<"\","<<rif<<","<<pais_origen_id<<","<<pais_procedencia_id<<","<<nacional<<","<<planta_id_id<<std::endl;	
		}
	}

	else
	file<<"00"<<",\""<<"NO ENCONTRADO"<<"\","<<"00"<<","<<"NONE"<<","<<"NONE"<<","<<"V"<<","<<"00000000"<<std::endl;		

	q->clear();
	file.close();

}

void Fetch::producto_t_insumo(std::string producto)
{

	//std::size_t found;
	std::string id,insumo_id_id,producto_id_id,anho,cantidad,activo,fecha_insercion,query;
	
	std::ofstream file;

	file.open("produccion_producto_t_insumo.csv", std::ios::app);

	query=(std::string)"SELECT id,insumo_id_id,producto_id_id,anho,cantidad,activo,fecha_insercion FROM produccion_producto_t_insumo WHERE producto_id_id="+"\'"+producto+"\'";
	
	q->exec(query);
	q->reset();



	while(q->hasResult() && q->next())
	{

	 id=q->getValue(0);
	 insumo_id_id=q->getValue(1);
	 producto_id_id=q->getValue(2);
	 anho=q->getValue(3);
	 cantidad=q->getValue(4);
	 activo=q->getValue(5);
	 fecha_insercion=q->getValue(6);


	produccion_insumo(insumo_id_id);	

	file<<id<<",\""<<insumo_id_id<<"\","<<producto_id_id<<","<<anho<<","<<cantidad<<","<<activo<<","<<fecha_insercion<<std::endl;	
	//file<<id<<",\""<<insumo_id_id<<"\","<<producto_id_id<<","<<anho<<","<<cantidad<<","<<std::endl;	
	}
	q->clear();
	file.close();

}

void Fetch::socios(std:: string rif)
{

	int i=0;
	std::size_t found;
	std::string id, unidad_economica_rif, rif_pasaporte, nombre, nacionalidad,tipo_persona_id, porcentaje;
	
	std::ofstream file;

	file.open("unidadecon_socio.csv", std::ios::app);

	query=(std::string) "SELECT id, unidad_economica_rif, rif_pasaporte, nombre, nacionalidad,tipo_persona_id, porcentaje FROM unidadecon_socio WHERE unidad_economica_rif="+"\'"+rif+"\'";

	q->exec(query);
	q->reset();

	if(q->hasResult())
	{
		while(q->hasResult() && q->next())
		{
	
		 id=q->getValue(0);
		 unidad_economica_rif=q->getValue(1);
		 rif_pasaporte=q->getValue(2);
		 nombre=q->getValue(3);
		 nacionalidad=q->getValue(4);
		 tipo_persona_id=q->getValue(5);
	 	 porcentaje=q->getValue(6);
	        
		 found=nombre.find("\"");
	
			while(found!=std::string::npos)
			{
		
			 nombre.erase(found,1);
			 found=nombre.find("\"");
			
			}
	
		file<<std::to_string(i)<<","<<id<<","<<unidad_economica_rif<<","<<rif_pasaporte<<",\""<<nombre<<"\","<<nacionalidad<<","<<tipo_persona_id<<","<<porcentaje<<std::endl;	
		i++;
		}
	}
	else
	
	//file<<std::to_string(i)<<","<<"00"<<","<<"0000000000"<<","<<""<<",\""<<"NONE"<<"\","<<"NONE"<<","<<"NONE"<<","<<"00"<<std::endl;
	

	q->clear();
	file.close();

}


void Fetch::aditionalUE(std::string planta)
{
		std::string query=(std::string)"SELECT ue_rif FROM unidadecon_subunidad_economica WHERE id="+"\'"+planta+"\'";
		q->exec(query);
		
		while(q->hasResult() && q->next())
		{
		 unidades_economicas(q->getValue(0));
		}
		

}


#endif



