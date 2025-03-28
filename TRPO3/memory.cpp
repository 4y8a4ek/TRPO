#include <string>
template <class Number>
class Memory
{
private:
    Number numb;
    bool IsUsing = false;
public:
    void set( const Number &nb){
        numb = nb;
        IsUsing = true;
    }
    Number get (){
        if(IsUsing){
            return numb;
        }
        Number nb(" ", 16);
        return nb;
    }

    void clear(){
        IsUsing = false;
    }

    void add(const Number &nb){
        if(IsUsing){
            numb = numb + nb;
        }
        else{
            set(nb);
        }
    }

};
