/*****************************************************
             PROJECT  : MALT
             VERSION  : 1.2.2
             DATE     : 10/2020
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/****************************************************/
//Deps
var fs          = require('fs');
var Args        = require('arg-parser');

/****************************************************/
Reset = "\x1b[0m"
FgRed = "\x1b[31m"
FgGreen = "\x1b[32m"

//internal classes
var MaltProject = require('./server-files/MaltProject.js');

/****************************************************/
//intenral cache for computed data which take a while to built
var maltCache = new Object();

/****************************************************/
//Manage args
args = new Args('test', '1.0', 'Simple integration test','');
//define args
args.add({ name: 'function', desc: 'Function to search', switches: [ '-f', '--function'], value: 'function', required: true });
args.add({ name: 'input', desc: 'input file from MALT into JSON format', switches: [ '-i', '--input-file'], value: 'file', required: true });
if (!args.parse()) 
{
	console.error("Invalid parameters, please check with -h");
	process.exit(1);
}

/****************************************************/
//load file
var maltProject = new MaltProject(args.params.input);

/****************************************************/
function stackString(project, func)
{
	//vars
	var res = "";

	//extract
	var tmp = project.getFlatFunctionProfile();
	
	// convert stack to flat string
	tmp.forEach(entry => {
		if (entry.function == func) {
			out = {
				"alloc": entry.total.alloc,
				"free": entry.total.free,
			};
			console.log(JSON.stringify(out, null, 4));
		}
	});

	//ok
	return res;
}

setTimeout(function() {
	console.log("============================================================");
	var out = stackString(maltProject, args.params.function);
	console.log(out);
}, 500);
