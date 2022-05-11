const socket = io()
socket.emit("message", "hello")
socket.on("message", function(msg){
	console.log(msg)
}) 

socket.on("disatncia", function(msg){
	console.log("distancia" + msg)
})

TESTER = document.getElementById('chart');

function random_vals(){
	return Math.random()
}

Plotly.newPlot( TESTER, [{
	y: [random_vals()],
	type: "line"
}]);

setInterval(function(){
	Plotly.extendtracesf
}
,200)	

console.log(Morris.Line.data)    

