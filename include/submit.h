#include <Arduino.h>
String submit = R"***(
    <!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">

    <style>
        body {
            background-color: rgb(41, 39, 39);
            font-family: 'Courier New', Courier, monospace;
            font-size: small;
        }

        .container {
            height: 40vh;
            display: flex;
            flex-direction: column;
            justify-content: space-between;
        }

        .titulo {
            height: 10vh;
            color: aliceblue;
            margin-top: 9%;
            margin-left: 2%;
            font-size: larger;
        }

        .nombre {
            background-color: rgb(41, 39, 39);
            display: flex;
            flex-direction: column;
            justify-content: space;
            height: 4vh;
            justify-content: space-between;
        }

        .pasword {
            background-color: rgb(41, 39, 39);
            display: flex;
            flex-direction: column;
            justify-content: space;
            height: 7vh;
            justify-content: start;
            margin-top: 9%;
        }

        .item {
            background-color: rgb(41, 39, 39);
            height: 4vh;
            font-size: medium;
            font-size: large;
            color: rgb(125, 171, 211);
        }

        .input {
            background-color: rgb(41, 39, 39);
            height: 4vh;
            font-size: medium;
            font-size: large;
            color: aliceblue;
            border-color: rgba(240, 248, 255, 0.493);
            border-style: solid;
        }
        .submit{
            display: flex;
            justify-content: space-around;
            margin-top: 2%;
        }
        .Control_link{
            text-align: center;
            padding-top: 0.5em;
            border: solid;
            border: 0px;
            background-color: rgb(100, 4, 156);
            border-radius: 10%;
            color: rgb(255, 255, 255);
            font-family: 'Courier New', Courier, monospace;
            font-size: large;
            width: 49%;
        }

        .button {
            height: 40px;
            width: 100px;
            border: solid;
            border: 0px;
            background-color: rgb(1, 211, 211);
            border-radius: 10%;
            color: rgb(255, 255, 255);
            margin-left: 2%;
            font-family: 'Courier New', Courier, monospace;
            font-size: large;
        }

    </style>

    <title>Submit</title>
</head>

<body>
    <div class="container">
        <div class="titulo">
            <h1>Configuración </h1>
        </div>
        <div class="nombre">
            <div class="item">
                <b class="item">
                    Nombre de la red
                </b>
            </div>
            <input class=input id="nombre" type="text" placeholder="Nombre de la red">
        </div>
        <div class="pasword">
            <div class="item">
                <b class="item">
                    Contraseña
                </b>
            </div>
            <input class=input id="contrasena" type="password" placeholder="Contraseña">
        </div>
        <div class="container_submits">
            <div class="submit">
                    <input id=submit class=button type="button" value="Enter" placeholder="Enter">
                <a href="/index" class="Control_link" >
                    Usar sin red
                </a>
                
            </div>
        </div>
    </div>

    <script>

        let socket = new WebSocket("ws://"+location.hostname+":81")   
        //Datos recividos del servidor:
        socket.onmessage = function (event) {
            alert(`"Mensaje: ${event.data}"`)
        }

        socket.onclose = function(event) {
            if (event.wasClean) {
                alert(`[close] Conexión cerrada limpiamente, código=${event.code} motivo=${event.reason}`);
            } else {
                // ej. El proceso del servidor se detuvo o la red está caída
                // event.code es usualmente 1006 en este caso
                alert('Intentando conectar, si no se puede recargue la pagina y vuelva a poner las credenciales. El led se apagará cuando se conecte a la red');
                recconect(); 
            }
        }

        //Envio de datos
        let submit = document.getElementById("submit")
        submit.addEventListener("click", () => {
            var nombre = document.querySelector("#nombre").value
            var pasword = document.querySelector("#contrasena").value
            values = [nombre, pasword];
            socket.send(values); 
            console.log(values); 
        })
    </script>
</body>

</html>

</html>
)***"; 