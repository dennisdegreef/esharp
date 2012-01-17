/*
Numbers
*/

//Integers are created by specifying their value in decimal notation

a = 1;
b = 2;

stdout.println("==integer==\n", "a=", a, " b=", b, "\n");

stdout.println("a+b=", a+b);
stdout.println("a-b=", a-b, " <- Integers are always signed.");
stdout.println("a*b=", a*b);
stdout.println("a/b=", a/b, " <- What? I always though half is about nothing but...");
stdout.println("a%b=", a%b);

stdout.println();


//Real numbers are created by specifying their value in decimal notation using a dot

a = 1.0;
b = 2.0;

stdout.println("==real==\n", "a=", a, " b=", b, "\n");

stdout.println("a+b=", a+b);
stdout.println("a-b=", a-b);
stdout.println("a*b=", a*b);
stdout.println("a/b=", a/b, " <- The difference is here!!!");

stdout.println();


//Octet numbers are created by specifying their value in hexadecimal notation

a = 0x01;
b = 0x02;

stdout.println("==octet==\n", "a=", a, " b=", b, "\n");

stdout.println("a+b=", a+b);
stdout.println("a-b=", a-b, " <- And they are always positive...");
stdout.println("a*b=", a*b);
stdout.println("a/b=", a/b);
stdout.println("a%b=", a%b);

stdout.println();
