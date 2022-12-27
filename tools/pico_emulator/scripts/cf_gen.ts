const file = JSON.parse(Deno.readTextFileSync("build/compile_commands.json")) as [ {
	directory: string,
	command: string,
	file: string
} ];

const includes: string[] = [];
for (const p of file) {
	for (const c of p.command.split(" ")) {
		if (c.startsWith("-I")) {
			if (!includes.includes(c)) {
				includes.push(c);
			}
		}
	}
}
console.log("Found " + includes.length + " includes!");
Deno.writeTextFileSync("compile_flags.txt", includes.join("\n"));
