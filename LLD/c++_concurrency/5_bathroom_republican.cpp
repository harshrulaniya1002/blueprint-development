// a bathroom is designed in such a way that:
// 1) male and female both can use
// 2) no men and women in bathroom at same time
// 3) there cannot be more than 3 employees in bathroom at one time
// dont worry about starvation for now.(agar 3 female and 1 male hn, male ko moka nhi mil rha , har baar jaise hi khali hone wala hota, new female entered)
// may be we can check last one is female than next male will go, vice versa.
// or may be like criteria something if male is not around 2 sec , than female go, to remove starvation


mutex mtx;
condition_variable cv;
void maleusebathroom(string name){
    unqiue_lock<mutex> l(mtx);
    cv.wait(l,[&]{
        return (counter >=0 && counter <= 2);
    });

    counter++;

    cout<<counter<<" "<<"Male Entered"<<"\n";
    l.unlock();

    cout<<"Make used the bathroom"<<"\n";
    l.lock();
    counter--;
    l.unlock();

    cv.notify_all();
}

void femaleusebathroom(string name){
    unqiue_lock<mutex> l(mtx);
    cv.wait(l,[&]{
        return (counter <=0 && counter >= -2);
    });

    counter--;

    cout<<counter<<" "<<"Male Entered"<<"\n";
    l.unlock();

    cout<<"Make used the bathroom"<<"\n";
    l.lock();
    counter++;
    l.unlock();

    cv.notify_all();
}