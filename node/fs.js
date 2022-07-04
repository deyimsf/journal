const fs = require('fs')
fs.stat('/My/workspace/git/journal/node/cc.js', (err,stats)=>{
	if(err){
		console.error(err)
		return
	}
	
	console.log(stats.isFile())
})


