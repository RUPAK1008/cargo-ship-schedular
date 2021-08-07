///////////////////////////////////////////////////////////////////////////////////////////
//                            CARGO SHIP SCHEDULER PROGRAM                               //
//                            (DURATION:-  JUN/2020-JUL/2021)                            //
//                              BY:- RUPAK DAS (18045078)                                //
///////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                      //
//                            HEADER FILES THAT ARE USED                                //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////
#include<bits/stdc++.h>
// #include<>
#include <windows.h>    //  header file for gotoxy
#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
//                        CLASS DEFINITION FOR CARGO DETAILS                              //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////

class cargo_info{
    string cargo_name;
    int arrival_time;
    int change_arrival_time;
    int loading_time;
    int departion_cargo_time;
    char cargo_type;
    int station_port;
    public:

    // funtion to convert string time into interger formate
     int Cal_Str_To_Int(string arritime){
       int time=0;
       if(arritime[6]=='p')
       time=1440;
       time=time+(arritime[0]-'0')*600;
       time=time+(arritime[1]-'0')*60;
       time=time+(arritime[3]-'0')*10;
       time=time+(arritime[4]-'0')*1;
       return time;
     }
      // Enter the cargo details;
     void cargo_get(){
       cout<<"Please Enter details related to Cargo for port scheduling\n\n\n";
       cin.ignore(numeric_limits<streamsize>::max(), '\n');
       cout<<"Enter the Name of the Cargo ship:-\n";
       string cName;
       getline(cin,cName);
       this-> cargo_name=cName;
       cout<<"\nEnter the Arrival Time of Cargo ship (in format of hh:mm:am):\n";
       string arritime;
       cin>>arritime;
       this->arrival_time=this->change_arrival_time=Cal_Str_To_Int(arritime);
       cout<<"\nEnter the approximate loading/deloading time it going to take (in min):-\n";
       int ltime;
       cin>>ltime;
       this->loading_time=ltime;
       cout<<"\nEnter cargo size ('S' for small , 'M' for medium , 'L' for large):-\n";
       char ctype;
       cin>>ctype;
       this->cargo_type=ctype;

     }

     // provide cargo size
     char cargo_size(){
         char cargo_size=cargo_type;
         return cargo_size;
     }

     // provide arrival time
     int get_arival_time(){
         int n=arrival_time;
         return n;
     }

     // provide loading time
     int job_time(){
         int n=loading_time;
         return n;
     }
     // insert departure time and station port
     void dt_st_size(int dt,int st,char si){
         departion_cargo_time=dt;
         station_port=st;
         cargo_type=si;
     }
     // get departure time
     int get_dt(){
         return departion_cargo_time;
     }
     // change arrival time
     void change_arrival_time_set(int cat){
         change_arrival_time=cat;
     }
     //get station port
     int get_st(){
        return station_port;
     }
      // show the details of the entered cargo details
     void show_details(){
       cout<<cargo_name<<'\t';
       cout<<arrival_time<<'\t';
       cout<<change_arrival_time<<'\t';
       cout<<loading_time<<'\t';
       cout<<departion_cargo_time<<'\t';
       cout<<cargo_type<<'\t';
       cout<<station_port<<'\t';
     }

};
////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
//                        CLASS DEFINITION FOR CARGO DETAILS                              //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////
class cargo_schedular{
    int n;
    cargo_info cargo_list[10005];
    set<int> small_port,medium_port,large_port;
public:
    // take all the details and store into class cargo_info to apply schuling of
    // all the cargos and assign them their designated port and time
    void cargo_schedular_start(int n){
        this->n=n;
        for(int i=0;i<n;i++){
            // clear screen
            cargo_list[i].cargo_get();
        }
        empty_port_settor();
        SJFschedule(n);
        // clear screen
        for(int i=0;i<n;i++){
            cargo_list[i].show_details();
            cout<<endl;
        }
    }

    // empty port settor
    void empty_port_settor(){
        for(int i=1;i<=10;i++){
            small_port.insert(i);
            medium_port.insert(i);
            large_port.insert(i);
        }
    }
    // long job first sort comparator
    static bool compSJF(cargo_info &a,cargo_info &b){
        if(a.get_arival_time()!=b.get_arival_time())
        return a.get_arival_time()<b.get_arival_time();
        return a.job_time()<b.job_time();

    }
    // empty the port force fully funtion
    pair<char,int> find_cargo(char c1,char c2,queue<cargo_info> time_table){
        int dt=1e9,k=time_table.size();
        while(k>0){
            k--;
            cargo_info temp=time_table.front();
            time_table.pop();
            int temp_time=temp.get_arival_time();
            if(temp.cargo_size()==c1||temp.cargo_size()==c2)
            dt=min(dt,temp_time);
            time_table.push(temp);
        }
        bool f1=false,f2=false;
        k=time_table.size();
        while(k>0){
            k--;
            cargo_info temp=time_table.front();
            time_table.pop();
            int temp_time=temp.get_arival_time();
            if(dt!=temp_time||(temp.cargo_size()!=c1&&temp.cargo_size()!=c2)){
                time_table.push(temp);
                continue;
            }
            if(temp.cargo_size()==c1)
                f1=true;
            else
                f2=true;
        }
        if(f1)
            return make_pair(c1,dt);
        return make_pair(c2,dt);
    }


    // it schedule the cargo according to their arrival time and register their port for stationing
    // using concept of shortest job first scheduling algorithms with the help of multi processor
    // theory and parallel procesing.
    void SJFschedule(int n){
         this-> n = n;
         queue<cargo_info> time_table;
         sort(cargo_list,cargo_list+n,compSJF);
         int i=0,present_time;
         for(int i=0;i<n;i++){
            cargo_info temp=cargo_list[i];
            char cargo_size=temp.cargo_size();
            int check_time=temp.get_arival_time();
            int k=time_table.size();
            while(k>0){
                k--;
                cargo_info temp2=time_table.front();
                time_table.pop();
                if(temp2.get_dt()>check_time){
                    time_table.push(temp2);
                    continue;
                }
                char portS=temp2.cargo_size();
                if(portS=='S'){
                    int portN=temp2.get_st();
                    small_port.insert(portN);
                }else if(portS=='M'){
                    int portN=temp2.get_st();
                    medium_port.insert(portN);
                }else if(portS=='L'){
                    int portN=temp2.get_st();
                    large_port.insert(portN);
                }
            }
            //if it is small size cargo
            if(cargo_size=='S'){
                if(small_port.size()){
                    auto port=small_port.begin();
                    small_port.erase(port);
                    int dt=temp.get_arival_time()+temp.job_time();
                    cargo_list[i].dt_st_size(dt,*port,'S');
                }else if(medium_port.size()){
                    auto port=medium_port.begin();
                    medium_port.erase(port);
                    int dt=temp.get_arival_time()+temp.job_time();
                    cargo_list[i].dt_st_size(dt,*port,'M');
                }else{
                    pair<char,int> p=find_cargo('S','M',time_table);
                    if(p.first=='S'){
                        if(small_port.size()){
                            auto port=small_port.begin();
                            small_port.erase(port);
                            int dt=p.second+temp.job_time();
                            cargo_list[i].dt_st_size(dt,*port,'S');
                            cargo_list[i].change_arrival_time_set(p.second);
                        }
                    }else{
                        if(medium_port.size()){
                            auto port=medium_port.begin();
                            medium_port.erase(port);
                            int dt=p.second+temp.job_time();
                            cargo_list[i].dt_st_size(dt,*port,'M');
                            cargo_list[i].change_arrival_time_set(p.second);
                        }
                    }
                }
            }else
            // if it is medium size cargo
            if(cargo_size=='M'){
                if(medium_port.size()){
                    auto port=medium_port.begin();
                    medium_port.erase(port);
                    int dt=temp.get_arival_time()+temp.job_time();
                    cargo_list[i].dt_st_size(dt,*port,'M');
                }else if(large_port.size()){
                    auto port=large_port.begin();
                    large_port.erase(port);
                    int dt=temp.get_arival_time()+temp.job_time();
                    cargo_list[i].dt_st_size(dt,*port,'L');
                }else{
                    pair<char,int> p=find_cargo('M','L',time_table);
                    if(p.first=='M'){
                        if(medium_port.size()){
                            auto port=medium_port.begin();
                            medium_port.erase(port);
                            int dt=p.second+temp.job_time();
                            cargo_list[i].dt_st_size(dt,*port,'M');
                            cargo_list[i].change_arrival_time_set(p.second);
                        }
                    }else{
                        if(large_port.size()){
                            auto port=large_port.begin();
                            large_port.erase(port);
                            int dt=p.second+temp.job_time();
                            cargo_list[i].dt_st_size(dt,*port,'L');
                            cargo_list[i].change_arrival_time_set(p.second);
                        }
                    }
                }
            }else
            // if it is large size cargo
            if(cargo_size=='L'){
                if(large_port.size()){
                    auto port=large_port.begin();
                    large_port.erase(port);
                    int dt=temp.get_arival_time()+temp.job_time();
                    cargo_list[i].dt_st_size(dt,*port,'L');
                }else{
                    pair<char,int> p=find_cargo('L','L',time_table);
                    if(large_port.size()){
                        auto port=large_port.begin();
                        large_port.erase(port);
                        int dt=p.second+temp.job_time();
                        cargo_list[i].dt_st_size(dt,*port,'L');
                        cargo_list[i].change_arrival_time_set(p.second);
                    }
                }
            }
         }
     }
};

COORD CursorPosition;
void gotoxy(int x,int y){
  CursorPosition.X=x;
  CursorPosition.Y=y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),CursorPosition);
}

int main(){
   //ios_base::sync_with_stdio(false);
	// cin.tie(0);cout.tie(0);

 //#ifndef ONLINE_JUDGE
 //    freopen("input.txt", "r", stdin);
 //    freopen("output.txt", "w", stdout);
 //#endif
  cout<<"ENTER NUMBER OF CARGO COMING TODAY:-  ";
  int n;
  cin>>n;
  cout<<"\nENTER THE DETAILS OF ALL CARGO SHIP\n";
  cargo_schedular cargo;
  cargo.cargo_schedular_start(n);
  return 0;

}

