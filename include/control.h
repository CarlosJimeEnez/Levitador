#include <Arduino.h>
String control = R"***(
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>El inombrable</title>
    
        <style>
            .body{
                background-color: rgb(39, 41, 41);
            }
            .superior{
                border-radius: 0% 0% 15% 15% ;
                height: 7vh;
                background-color: #3b3b3b2a;
                display: flex;
                justify-content: center;
            }
            .container{
                background-color: rgb(39, 41, 41);
                display: flex;
                justify-content: space-around;
            }
            .header{
                background-color: rgb(39, 41, 41);
                display: flex;
                height: 30vh;
                align-items: center;
                justify-content: center;
            }
            .child{
                background-color: #315ce7;
                border-radius: 20%;
                width: 36%;
                height: 12vh;
                position: relative;
            }

            .child_flex{
                background-color: #315ce7;
                border-radius: 20%;
                height: 20vh;
                width: 25%;
                margin-top: 20px;
                position: relative;
            }

            .play{
                background-color: rgba(132, 0, 255, 0);
                border: 29px;
                border-radius: 9%;
                border-style: solid;
                border-color: rgba(255, 255, 255, 0) rgba(0, 0, 255, 0) rgba(255, 0, 0, 0) rgb(245, 245, 239);
                height: 0;

                position: absolute;
                top: 50%;
                left: 50%;
                margin-top: -30px;
                margin-left: -13px;
                border-right-width: 0px;
            }
            
            .play_righ{
                background-color: rgba(132, 0, 255, 0);
                border: 29px;
                border-radius: 9%;
                border-style: solid;
                border-style: double;
                border-color: rgba(255, 255, 255, 0) rgba(0, 0, 255, 0) rgba(255, 0, 0, 0) rgb(245, 245, 239);
                height: 0;
                
                position: absolute;
                top: 50%;
                left: 50%;
                margin-top: -30px;
                margin-left: -13px;
                border-right-width: 0px;
            }

            .play_left{
                background-color: rgba(132, 0, 255, 0);
                border: 29px;
                border-radius: 9%;
                border-style: solid;
                border-style: double;
                border-color: rgba(255, 255, 255, 0) rgba(0, 0, 255, 0) rgba(255, 0, 0, 0) rgb(245, 245, 239);
                height: 0;

                position: absolute;
                right: 50%;
                top: 50%;
                margin-right: -13px;
                margin-top: -30px;
                rotate: 180deg;
            }

            .stop{
                background-color: 5D8BF4;
                border: 20px;
                border-style: solid;
                border-style: double;
                border-width: 0px 0px 0px 39px;
                border-color: rgb(255, 255, 255) rgb(255, 255, 255) rgb(255, 255, 255) rgb(255, 255, 255);
                height: 43px;
                position: absolute;
                top: 50%;
                left: 50%;
                margin-top: -25px;
                margin-left: -19px;
            }
            
            .background{
                border-radius: 15% 15% 0 0 ;
                margin-top: 55%;
                height: 10vh;
                background-color: #3b3b3b2a;
                display: flex;
                justify-content: center;
            }


            input[type=range] {
                height: 33px;
                -webkit-appearance: none;
                margin: 10px 0;
                width: 100%;
                }
                
            input[type=range]:focus {
            outline: none;
            }
            input[type=range]::-webkit-slider-runnable-track {
            width: 100%;
            height: 13px;
            cursor: pointer;
            box-shadow: 0px 0px 0px #000000;
            background: #315ce7;
            border-radius: 25px;
            border: 0px solid #000101;
            }
            input[type=range]::-webkit-slider-thumb {
            box-shadow: 0px 0px 0px #000000;
            border: 0px solid #A978FF;
            height: 27px;
            width: 43px;
            border-radius: 20px;
            background: #654F5D;
            cursor: pointer;
            -webkit-appearance: none;
            margin-top: -7px;
            }
            input[type=range]:focus::-webkit-slider-runnable-track {
            background: #315ce7;
            }
            input[type=range]::-moz-range-track {
            width: 100%;
            height: 13px;
            cursor: pointer;
            box-shadow: 0px 0px 0px #000000;
            background: #315ce7;
            border-radius: 25px;
            border: 0px solid #000101;
            }
            input[type=range]::-moz-range-thumb {
            box-shadow: 0px 0px 0px #000000;
            border: 0px solid #A978FF;
            height: 27px;
            width: 43px;
            border-radius: 20px;
            background: #654F5D;
            cursor: pointer;
            }
            input[type=range]::-ms-track {
            width: 100%;
            height: 13px;
            cursor: pointer;
            background: transparent;
            border-color: transparent;
            color: transparent;
            }
            input[type=range]::-ms-fill-lower {
            background: #315ce7;
            border: 0px solid #000101;
            border-radius: 50px;
            box-shadow: 0px 0px 0px #000000;
            }
            input[type=range]::-ms-fill-upper {
            background: #315ce7;
            border: 0px solid #000101;
            border-radius: 50px;
            box-shadow: 0px 0px 0px #000000;
            }
            input[type=range]::-ms-thumb {
            margin-top: 1px;
            box-shadow: 0px 0px 0px #000000;
            border: 0px solid #A978FF;
            height: 27px;
            width: 43px;
            border-radius: 20px;
            background: #654F5D;
            cursor: pointer;
            }
            input[type=range]:focus::-ms-fill-lower {
            background: #315ce7;
            }
            input[type=range]:focus::-ms-fill-upper {
            background: #315ce7;
            }
            
            #vel_range{
                background-color: rgb(39, 41, 41);
            }
        </style>
    
    </head>
    <body class="body">
        <div class="superior">  </div>
        <div class="header">
            <div class="child"> 
                <input type="button" class="play" onclick="play()">
            </div>
        </div>
    
        <div class="container">
            <input type="range" name="" id="vel_range">
        </div>

        <div class="container">
            <div class="child_flex">
                <input type="button" class="play_left" value="">
            </div>
            <div class="child_flex">
               <input type="button"  class="play_righ" value="">
            </div>
        </div>

        <div class="background">  </div>
       
    </body>
    
    
    <script>
        let socket = new WebSocket("ws://"+location.hostname+":81")

        //Datos recividos del servidor:
        socket.onmessage = (event) => { 
            alert(`"Datos recividos: ${event.data}"`)
        }  

        socket.onclose = function(event) {
            if (event.wasClean) {
                alert(`[close] Conexión cerrada limpiamente, código=${event.code} motivo=${event.reason}`);
            } else {
                // ej. El proceso del servidor se detuvo o la red está caída
                // event.code es usualmente 1006 en este caso
                alert('[close] La conexión se cayó');

            }
        };
                    
        range = document.querySelector("#vel_range"); 
        range.onchange = ()=>{
            values = ["range", range.value]; 
            socket.send(values); 
        }

        var toggle = 0;
        //Toggle function: 
        function play(value){
            
            if(toggle == 0){
                document.querySelector(".play").className = "stop";
                socket.send("play")
                toggle = 1
            }
            else{
                document.querySelector(".stop").className = "play";
                socket.send("stop")
                toggle = 0
            }
        }
    </script>
    </html>
    )***";
