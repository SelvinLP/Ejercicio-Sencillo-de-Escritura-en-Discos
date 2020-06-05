#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <QDir>
using namespace std;

struct Alumnos_Profesores{
    int Tipo;
    char Apellido1[21];
    char Apellido2[21];
    char Nombre[20];
    char Codigo[11];
    int NotaOSeccion;
};
void CrearArchivo();
void ReporteRegistro(int CanidadDatos);
void ModificarArchivo(Alumnos_Profesores nuevomod,int CantidadDatos);
int InsertarArchivo(Alumnos_Profesores nuevos, int CantidadDatos);
int main()
{
    int CantidadInsertado=0;
    CrearArchivo();
    //menu
    bool ciclomenu=true;
    while (ciclomenu) {
        int opcion=0;
        cout<<"1. Ingresar un usuario "<<endl;
        cout<<"2. Modificar un usuario "<<endl;
        cout<<"3. Reporte "<<endl;
        cout<<"4. Salir "<<endl;
        cin>>opcion;
        if(opcion==4){ciclomenu=false;}
        switch (opcion) {
        case 1:
            //Escribir
            Alumnos_Profesores nuevo;
            cout<<"1. Ingresar Tipo"<<endl;
            cin>>nuevo.Tipo;
            cout<<"2. Ingresar Apellido 1"<<endl;
            cin>>nuevo.Apellido1;
            cout<<"3. Ingresar Apellido 2"<<endl;
            cin>>nuevo.Apellido2;
            cout<<"4. Ingresar Nombre"<<endl;
            cin>>nuevo.Nombre;
            cout<<"5. Ingresar Codigo"<<endl;
            cin>>nuevo.Codigo;
            if(nuevo.Tipo==0){
                cout<<"4. Ingresar Nota"<<endl;
                cin>>nuevo.NotaOSeccion;
            }else{
                cout<<"4. Ingresar Seccion"<<endl;
                cin>>nuevo.NotaOSeccion;
            }
            //inserta

            CantidadInsertado=InsertarArchivo(nuevo,CantidadInsertado);
            break;

        case 2:
            Alumnos_Profesores nuevo2;
            cout<<"5. Ingresar Codigo"<<endl;
            cin>>nuevo2.Codigo;
            cout<<"2. Ingresar Apellido 1"<<endl;
            cin>>nuevo2.Apellido1;
            cout<<"3. Ingresar Apellido 2"<<endl;
            cin>>nuevo2.Apellido2;
            cout<<"4. Ingresar Nombre"<<endl;
            cin>>nuevo2.Nombre;
            cout<<"4. Ingresar Nota o Seccion"<<endl;
            cin>>nuevo2.NotaOSeccion;
            ModificarArchivo(nuevo2,CantidadInsertado);
            break;

        case 3:
            //Reporte

            ReporteRegistro(CantidadInsertado);
            break;

        }
    }


    //Cierra Aplicacion

    return 0;
}

void CrearArchivo(){
    //creacmos comando para ejecutar en consola
    QDir dir("si/"); if (!dir.exists()) { dir.mkpath("."); }

    string CadenaComando=" dd if=/dev/zero of=si/alu\\ mnsos_[Carne].dat bs=2048 count=1";
    //creamos un archivo
    system(CadenaComando.c_str());

}


void ReporteRegistro(int CantidadDatos){

    FILE *arch;
    arch=fopen("alumnos_[Carne].dat","rb");
    if (arch==NULL){
        exit(1);
    }
    Alumnos_Profesores Nodo_AP;
    for(int x=0;x<CantidadDatos;x++){
        fread(&Nodo_AP, sizeof(Alumnos_Profesores), 1, arch);
        cout<<"----- NUMERO DE REGISTRO "<<x+1<<" -----"<<endl;
        cout<<"-Codigo \t"<<Nodo_AP.Codigo<<endl;
        cout<<"-Tipo \t"<<Nodo_AP.Tipo<<endl;
        cout<<"-Apellido 1 \t"<<Nodo_AP.Apellido1<<endl;
        cout<<"-Apellido 2 \t"<<Nodo_AP.Apellido2<<endl;
        cout<<"-Nombre \t"<<Nodo_AP.Nombre<<endl;
        if(Nodo_AP.Tipo==0){
            cout<<"-Nota \t"<<Nodo_AP.NotaOSeccion<<endl;
        }else{
            cout<<"-Seccion \t"<<Nodo_AP.NotaOSeccion<<endl;
        }
    }
    fclose(arch);

}
void ModificarArchivo(Alumnos_Profesores nuevomod,int CantidadDatos){
    FILE *arch;
    arch=fopen("alumnos_[Carne].dat","r+b");
    if (arch==NULL){
        exit(1);
    }
    Alumnos_Profesores Nodo_AP;
    for(int x=0;x<CantidadDatos;x++){
        fread(&Nodo_AP, sizeof(Alumnos_Profesores), 1, arch);
        if(strcmp(nuevomod.Codigo,Nodo_AP.Codigo) == 0){
            nuevomod.Tipo=Nodo_AP.Tipo;
            //obtenemos posicion
            int pos=ftell(arch)-sizeof(Alumnos_Profesores);
            fseek(arch,pos,SEEK_SET);
            //modificamos
            fwrite(&nuevomod, sizeof(Alumnos_Profesores), 1, arch);
            cout<<"MODIFICADO CORRECTAMENTE    "<<nuevomod.Codigo<<endl;
        }
    }
    fclose(arch);

}
int InsertarArchivo(Alumnos_Profesores nuevos,int CantidadDatos){


    FILE *archivo;
    if ((archivo = fopen("alumnos_[Carne].dat","r+b")) == NULL){
        exit(1);
    }
    //comprobacion de final
    fseek(archivo,0,SEEK_END);
    int final=ftell(archivo);
    cout<<"-Final "<<final<<endl;
    //controlador si supera el espacio
    bool existencia=false;
    int pos=CantidadDatos*sizeof (Alumnos_Profesores);
    int compFinal=pos+sizeof (Alumnos_Profesores);
    if(compFinal>final){
       existencia=true;
    }
    cout<<"Posicion "<<pos<<endl;
    //Comproabcion si ya existe

    Alumnos_Profesores Nodo_AP;
    for(int x=0;x<CantidadDatos;x++){
        fread(&Nodo_AP, sizeof(Alumnos_Profesores), 1, archivo);
        if(strcmp(nuevos.Codigo,Nodo_AP.Codigo) == 0){
            existencia=true;
        }

    }
    //insertamos o no
    fseek(archivo,pos,SEEK_SET);
    if(existencia){
        //existe no se crea
        if(compFinal>final){
            cout<<"YA NO HAY ESPACIO EN EL ARCHIVO    "<<nuevos.Codigo<<endl;
        }else{
            cout<<"ESTE CODIGO YA EXISTE    "<<nuevos.Codigo<<endl;
        }
    }else{
        fwrite(&nuevos, sizeof(Alumnos_Profesores), 1, archivo);
        CantidadDatos++;
    }
    cout<<"-Nueva Posicion "<<pos+sizeof (Alumnos_Profesores)<<endl;
    fclose(archivo);
    return CantidadDatos;
}
