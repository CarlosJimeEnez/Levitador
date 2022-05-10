const socket = io()
socket.emit("message", "hello") 
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

