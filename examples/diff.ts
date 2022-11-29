import getFiles from "https://deno.land/x/getfiles/mod.ts";
import { diffCharacters } from "https://deno.land/x/diff/mod.ts";

async function run(command: string) {
	console.log("Running: '" + command + "'");
	var proc = Deno.run({
		cmd: command.split(" "),
		cwd: "./",
		stdout: "piped",
		stderr: "piped",
	});
    var stdout = new TextDecoder().decode(await proc.output());
    proc.close();
    return stdout;
}

function print_diff(s1: string, s2: string) {
    let finalString = "";
    for (const character of diffCharacters(s1, s2)) {
        if (character.wasRemoved) {
            // print red if removed without newline
            finalString += `\x1b[31m${character.character}\x1b[0m`;
        } else if (character.wasAdded) {
            // print green if added
            finalString += `\x1b[32m${character.character}\x1b[0m`;
        } else {
            // print white if unchanged
            finalString += `\x1b[37m${character.character}\x1b[0m`;
        }
    }
    console.log(finalString);
}

let files = getFiles("./");
for (let file of files) {
    if (file.ext == "bin") {
        let s2 = await run("phoenix-disasm " + file.path);
        let s1 = await run("phoenix-disasm " + "../../working/" + file.path);
        print_diff(s1, s2);
    }
}
