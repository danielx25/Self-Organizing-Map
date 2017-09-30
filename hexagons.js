function escalaGrises(act, min, max)
{
	x = ((act-min)/(max-min)*-255)+255;
	return "rgb("+x+", "+x+", "+x+")";
}


(function(){
    var canvas = document.getElementById('hexmap');
	var titulo = document.getElementById('red');
	var info = document.getElementById('infoRed');
	
	var min = Math.min.apply(null,lista_red);
	var max = Math.max.apply(null,lista_red);
	info.innerHTML = "filas: "+ ancho+ " columna: "+largo+ " min Act: "+min+ " max Act: "+max;
    canvas.width = 1920;
	canvas.height = 1080;
    
	//titulo.style.color = escalaGrises(80, min, max);
	
	var hexHeight,
        hexRadius,
        hexRectangleHeight,
        hexRectangleWidth,
        hexagonAngle = 0.523598776, // 30 degrees in radians
        sideLength = 15,
        boardWidth = ancho,
        boardHeight = largo;

    hexHeight = Math.sin(hexagonAngle) * sideLength;
    hexRadius = Math.cos(hexagonAngle) * sideLength;
    hexRectangleHeight = sideLength + 2 * hexHeight;
    hexRectangleWidth = 2 * hexRadius;
	//alert(frutas[0]);

    if (canvas.getContext){
        var ctx = canvas.getContext('2d');

        ctx.fillStyle = "#000000";
        ctx.strokeStyle = "#CCCCCC";
        ctx.lineWidth = 1;

        drawBoard(ctx, boardWidth, boardHeight);

        canvas.addEventListener("mousemove", function(eventInfo) {
            var x,
                y,
                hexX,
                hexY,
                screenX,
                screenY;

            x = eventInfo.offsetX || eventInfo.layerX;
            y = eventInfo.offsetY || eventInfo.layerY;

            
            hexY = Math.floor(y / (hexHeight + sideLength));
            hexX = Math.floor((x - (hexY % 2) * hexRadius) / hexRectangleWidth);

            screenX = hexX * hexRectangleWidth + ((hexY % 2) * hexRadius);
            screenY = hexY * (hexHeight + sideLength);

            ctx.clearRect(0, 0, canvas.width, canvas.height);

            drawBoard(ctx, boardWidth, boardHeight);

            // Check if the mouse's coords are on the board
            if(hexX >= 0 && hexX < boardWidth) {
                if(hexY >= 0 && hexY < boardHeight) {
                    ctx.fillStyle = "#000000";
                    drawHexagon(ctx, screenX, screenY, true);
                }
            }
        });
    }

    function drawBoard(canvasContext, width, height) {
        var i,
            j;
		var indice = 0;
        for(i = 0; i < width; ++i) {
            for(j = 0; j < height; ++j) {
				if(indice < width*height)
				{
					ctx.fillStyle = escalaGrises(lista_red[indice], min, max);
					ctx.strokeStyle = escalaGrises(lista_red[indice], min, max);
					ctx.lineWidth = 1;
					indice+=1;
				}
				
                drawHexagon(
                    ctx, 
                    i * hexRectangleWidth + ((j % 2) * hexRadius), 
                    j * (sideLength + hexHeight), 
                    true
                );
            }
        }
    }

    function drawHexagon(canvasContext, x, y, fill) {           
        var fill = fill || false;

        canvasContext.beginPath();
        canvasContext.moveTo(x + hexRadius, y);
        canvasContext.lineTo(x + hexRectangleWidth, y + hexHeight);
        canvasContext.lineTo(x + hexRectangleWidth, y + hexHeight + sideLength);
        canvasContext.lineTo(x + hexRadius, y + hexRectangleHeight);
        canvasContext.lineTo(x, y + sideLength + hexHeight);
        canvasContext.lineTo(x, y + hexHeight);
        canvasContext.closePath();

        if(fill) {
            canvasContext.fill();
        } else {
            canvasContext.stroke();
        }
    }

})();
