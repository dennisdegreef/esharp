/*
Containers
*/

//List is created by calling its new() method

a = list.new();
stdout.println("==list==\n", "a=", a, " <- Boring empty list.\n");

a = list.new(1, 2, 3, "a", "b", "c");
stdout.println("a=", a, "\n");

//  but you can append items to them later

a.append("xyz");
stdout.println("a=", a, "\n");

//  or you can delete items from them...

//a.del(0);
stdout.println("a=", a, "\n");

//  or you can set items from them...

a.set(0, "first");
stdout.println("a=", a, "\n");

//  and even get items from them...

stdout.println("a.get(a.len()-1)=", a.get(a.len()-1), "\n");



//Dict is created by calling its new() method (like with list but without any items)

a = dict.new();
stdout.println("==dict==\n", "a=", a, " <- Boring empty dict.\n");

//  you can set items from them...

a.set("Tom", 15);
a.set("John", 14);
a.set("Jane", 17);
a.set('Jane', 18);
a.set(45, 18);
a.set(44, 18);
a.set(43, 18);
a.set(42, 18);
stdout.println("a=", a, "\n");

//  or you can delete items from them...

//a.del("Tom");
//a.del(44);
stdout.println("a=", a, "\n");

//  and even get items from them...

stdout.println("a.get(\"John\")=", a.get("John"), "\n");
