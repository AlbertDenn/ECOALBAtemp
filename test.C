/* 

TEST DE USO DE DATOS DE BIENES Y COMPAÑIAS

*/

#include <stdio.h>
#include <iostream>
#include <dbQuery.H>
#include <string>
#include <tpl_dynDlist.H>
#include <tpl_dnode.H>

typedef struct {
 std::string nombre=" ",
             rif=" ",
	     planta=" ",
             tipo=" ";
}Company;



int main()
{
DBConnection conection;
DBQuery q(conection);
conection.open("file.txt");
std::string query;

Aleph::DynDlist <Company> uecon;



int i=0;
bool go;

q.exec("SELECT rif, nombre FROM unidadecon_unidad_economica");
q.reset();



while(q.hasResult() && q.next())
	{
	Company c;
	c.nombre=q.getValue(1);
	c.rif=q.getValue(0);
	uecon.insert(c);
	}
q.clear();

for(Aleph::Dnode<Company>::Iterator it(uecon);it.has_curr();it.next())
{

std::cout<<"nombre: "<<it.get_curr()->get_data().nombre<<std::endl<<"rif: "<<it.get_curr()->get_data().rif<<std::endl<<std::endl;


}
/*

std::cout<<endl<<"///////////////////////////PLANTAS/////////////////////////"<<std::endl<<std::endl;

for(i=0;i<100;i++)
{
query=(string)"SELECT nombre FROM unidadecon_subunidad_economica WHERE ue_rif="+(string)"\'"+(string)rif[i]+(string)"\'"+(string)" AND tipo_subunidad= 'Se'";


q.exec(query.c_str());
go=q.hasResult();
q.next();

if(go==1)
{
planta[i]=q.getValue(0);
std::cout<<"empresa: "<<nombre[i]<<std::endl<<"planta: "<<planta[i]<<std::endl;
go=0;
}
q.clear();

}
*/
return 0;


}
