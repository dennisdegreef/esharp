/*
Control flow statements
*/

//WHILE
i = 0;
while(i<10) {
	stdout.print(i, ".");
	i++;
}
stdout.println();

//FOR
for(i=0; i<10; i++) {
	stdout.print(i, ".");
}
stdout.println();

for(i=0, j=0; i<10; i++, j=j+2) {
	stdout.print(i, ".", j, "  ");
}
stdout.println();

//FOREACH
items = list.new(0,1,2,3,4,5,6,7,8,9);
foreach(i in items) {
	stdout.print(i, ".");
}
stdout.println();

//IF-ELSE
i=4;
if(i<5) {
	stdout.println("i<5");
} else {
	if(i>8) {
		stdout.println("i>8");
	} else {
		stdout.println("i>=5 & i<=8");
	}
}

//TRY-CATCH
try {
	i = 1 / 0;
} catch(exception e) {
	stdout.println("\n1 / 0 generated an error as it should: ", e, "\nmore info below:");
	stdout.println("---------------");
	e.print_trace(stdout);
	stdout.println("-----^Nice-----\n");
}

//CONTINUE & BREAK
items = list.new(0,1,2,3,4,5,6,7,8,9);
foreach(i in items) {
	if(i==3) {
		continue;
	}
	if(i==8) {
		break;
	}
	stdout.print(i, ".");
}
stdout.println();
