#ifndef _VECTOR_H
#define _VECTOR_H

class Vector{
private:
  double x;
  double y;
public:
  Vector(double argx=0, double argy=0);
  
  friend Vector operator + (Vector const &,Vector const &);
  friend Vector operator - (Vector const &,Vector const &);
  friend Vector operator / (Vector const &, double fac);
  friend Vector operator * (double fac,Vector const &);
 
  double getX();
  double getY();
  void setX(double argx){x=argx;}
  void setY(double argy){y=argy;}
};

#endif //_VECTOR_H