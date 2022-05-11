const socket = io()
var distancia = 0; 
socket.on("message", function(msg){
	distancia = parseInt(msg)
	$("#dist").text(distancia)  
}) 

socket.on("resultado", function(msg){
	var resultado = parseInt(msg)
	$("#resultado").text(resultado)
})

TESTER = document.getElementById('chart');

// function random_vals(){
// 	return Math.random()
// }

// Plotly.newPlot( TESTER, [{
// 	y: [random_vals()],
// 	type: "line"
// }]);

// setInterval(function(){
// 	Plotly.extendtracesf
// }
// ,200)	

//console.log(Morris.Line.data)    

