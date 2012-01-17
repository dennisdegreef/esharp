/*
Objects
*/

//Classes can be defined as a declaration:

class A {
	x() {
		stdout.println("x(0) was called.");
	}
}

A.x();

// or as an assignment:

B = class {
	y() {
		stdout.println("y(0) was called.");
	}
	z(a) {
		a.x();
	}
};

B.y();

// you can even do this:

B.z(A);

// or this:

B.z(class { x() { stdout.println("inline class x(0) was called."); }});

//Objects are created by using the new() method from the class

aobj = A.new();
B.z(aobj);

//Inheritance is specified using ':'

class C: A {
}

cobj = C.new();
B.z(cobj);

//There is inheritance inline
B.z(class :A{});

//And even this weird stuff (multiple inlined inheritance or something):
B.z(class:class:class:class:class:class:class:class:class:A{}{}{}{}{}{}{}{}{});

//Both classes and objects (you guessed they're not that different) have properties:

class D {
	readwrite a = 2.7182818311111;
	readonly b = 2.71828183;
	writeonly c = 1.3806504e-23;
	d = 3.14159265;
	printme(con) {
"""none <- D.printme(console <c>)
	Prints all the properties for class D to console <c>.
"""
		con.println("a=",a);
		con.println("b=",b);
		con.println("c=",c);
		con.println("d=",d);
	}
}

D.printme(stdout);

D.a(8);
stdout.println("a=",D.a());

///D.b(9); //<- this wouldn't work as b is readonly
stdout.println("b=",D.b());

D.c(10);
///stdout.println("c=",D.c()); //<- this wouldn't work as c is writeonly

///D.d(11); //<- this wouldn't work as d is private
///stdout.println("d=",D.d()); //<- this wouldn't work as d is private

stdout.println(D.doc());

stdout.println(object.doc());



