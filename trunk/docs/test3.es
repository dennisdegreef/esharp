/*
Strings
*/

//Str is created by specifying its value between double quotes

a = "abcdefghijklmnopqrstuvwxyz";

stdout.println("==str==\n", "a=", a, " <- Printed directly.\n");


//Data is created by specifying its value between single quotes

a = 'abcdefghijklmnopqrstuvwxyz';

stdout.println("==data==\n", "a=", a, " <- Data cannot be printed directly. Needs decoding...\n");
stdout.print("a.decode_utf8()=");
try {
	stdout.println(a.decode_utf8(), " <- See?");
} catch(exception e) {
	stdout.println("\nIt seems 'a' was having trouble decoding from UTF-8.");
}
stdout.println();


stdout.println("a.decode_ascii()=", a.decode_ascii(), " <- ASCII always decodes.");
stdout.println();


stdout.print("a.decode_ascii_strict()=");
try {
	stdout.println(a.decode_ascii_strict());
} catch(exception e) {
	stdout.println("\nIt seems 'a' was having trouble decoding from strict ASCII.");
}
stdout.println();

//Interaction between str and data is limited to encode/decode operations

a = "doublechanged";

stdout.println("a.encode_utf8().decode_utf8()=", a.encode_utf8().decode_utf8(), " <- Converted 'str' to encoded 'data' and back.\n");
