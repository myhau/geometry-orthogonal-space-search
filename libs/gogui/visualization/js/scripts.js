
var fileDataReader = null;
var inputDataReader = null;
var data = null;
var iterator = null;
var MARGIN = 10;
   		
var playing = false;
   		
function initJS() {

//	var h = $( window ).height();
//	var buttonBarHeight = 160;
//  var ratio = 7/5;
//   $(".canvas-area").css({"max-height": (h - buttonBarHeight).toString() + "px", "max-width": ((h - buttonBarHeight) * (ratio)).toString() + "px"});
//    console.log((h - buttonBarHeight) +" "+$( window ).width());
//    if(h*ratio < $( window ).width()) {
//        $(".canvas-area").css({"height": "126vh", "width": "100vh"});
//    }
//    else  {
//        $(".canvas-area").css({"height": (h - buttonBarHeight).toString() + "px", "width": ((h - buttonBarHeight) * (ratio)).toString() + "px"});
//        console.log("here");
//    }
//	  else
//       $(".canvas-area").css({"height": ((h - buttonBarHeight)*ratio2).toString() + "px", "width":((w - 30)).toString() + "px"});


	var windowContext = this;
	
	var callbackFunction = function ( _that ) {
		_that.data = new DataRepository( _that.fileDataReader.getPoints(), _that.fileDataReader.getLines() );
		_that.data.addSnapshots( _that.fileDataReader.getSnapshots() );
		_that.iterator = new SnapshotsIterator( _that.data );
		_that.updateLabel();
	};
	
	fileDataReader = new FileDataReader( 'data/data.json', callbackFunction, windowContext );
	
}
   		
function drawSnapshot( snapshot ) {
	
	$( ".my-canvas" ).empty();
	
	var pointHTMLTagOpen = "<circle id=\"";
	var pointHTMLTagClose = "\" cx=\"0\" cy=\"0\" r=\"0\" />";
	
	var lineHTMLTagOpen = "<line id=\"";
	var lineHTMLTagClose = "\" x1=\"0\" y1=\"0\" x2=\"0\" y2=\"0\" />"; 
	
	var snapshotPoints = snapshot.getPoints();
	var snapshotLines = snapshot.getLines();
	
	var scalingXPoints = function ( _x ) {
		
		var tmp;
		tmp = (_x-data.getXMin())/(data.getXMax() - data.getXMin());
		var w = $( ".canvas-area " ).width()-2*MARGIN;
		return (tmp * w)+MARGIN;	
	}
	
	var scalingYPoints = function ( _y ) {
		
		var tmp;
		tmp = (_y-data.getYMin())/(data.getYMax() - data.getYMin());
		var h = $( ".canvas-area " ).height()-2*MARGIN;
		return (tmp * h)+MARGIN;	
	}
	
	// draw points
	for( var i = 0; i < snapshotPoints.length; ++i ) {

        var p = data.getPointByIndex( snapshotPoints[i].pointID );
		
		var x = scalingXPoints(p.x);
		var y = scalingYPoints(p.y);
		
		var pointID = "point-" + i.toString();
		$( ".my-canvas" ).append( pointHTMLTagOpen + pointID + pointHTMLTagClose );
		
		
		$( "#" + pointID ).attr( 'cx', x.toString() );
		$( "#" + pointID ).attr( 'cy', y.toString() );
		$( "#" + pointID ).attr( 'r', "1" );
		$( "#" + pointID ).attr( 'fill', '#000000' );
		$( "#" + pointID ).attr( 'stroke-width', '3' );
		$( "#" + pointID ).attr( 'stroke', '#000000' );
		
		if( snapshotPoints[i].style == "active" ) {
			$( "#" + pointID ).attr( 'class', 'active-point' );
		} else if(snapshotPoints[i].style == "processed") {
			$( "#" + pointID ).attr( 'class', 'processed-point' );
		} else {
			$( "#" + pointID ).attr( 'class', 'normal-point' );
		}
		
	}
	
	
	// draw lines
	for( var i = 0; i < snapshotLines.length; ++i ) {
		
		var p1=data.getPointByIndex(data.getLineByIndex(snapshotLines[i].lineID).p1);
		var p2=data.getPointByIndex(data.getLineByIndex(snapshotLines[i].lineID).p2);
		
		var x1 = scalingXPoints(p1.x);
		var y1 = scalingYPoints(p1.y);
		var x2 = scalingXPoints(p2.x);
		var y2 = scalingYPoints(p2.y);

		//var x2 = data.getPointByIndex(data.getLineByIndex(snapshotLines[i].lineID).p2).x;
		//var y2 = data.getPointByIndex(data.getLineByIndex(snapshotLines[i].lineID).p2).y;
		
		var lineID = "line-" + i.toString();
		
		$( ".my-canvas" ).append( lineHTMLTagOpen + lineID + lineHTMLTagClose );
		$( "#" + lineID ).attr( 'x1', x1.toString() );
		$( "#" + lineID ).attr( 'y1', y1.toString() );
		$( "#" + lineID ).attr( 'x2', x2.toString() );
		$( "#" + lineID ).attr( 'y2', y2.toString() );
		
		if( snapshotLines[i].style == "normal" ) {
			$( "#" + lineID ).attr( 'class', 'normal-line' );
		} else {
			$( "#" + lineID ).attr( 'class', 'active-line' );
		}
	}
	
	$("body").html($("body").html());
}

function oneForward() {
	
	if( ! iterator.hasNext() ) {
		$('#forwardButton').attr("disabled", true);
	} else { 
		drawSnapshot( iterator.getNext() );
	}
	updateLabel();
}

function oneBackward() {
	if( ! iterator.hasPrevious() ) {
		$('#backwardButton').attr("disabled", true);
	} else { 
		drawSnapshot( iterator.getPrevious() );
	}
	updateLabel();
}

function reset() {
	$( ".my-canvas" ).empty();
	iterator.reset();
	updateLabel();
}

function updateLabel() {
	 $( ".counterLabel" ).empty();
	 var labelText = "";
	 labelText = "" + ( iterator.getSnapshotIndex() + 1 ).toString() + " / " + data.getNumberOfSnapshots().toString();
	 $( ".counterLabel" ).append( labelText );
}

function play() {

	if( playing ) {
		$( "#playButton" ).html('Odtwarzaj');
	} else {
		$( "#playButton" ).html('Zatrzymaj');
		animationPlayer();
	}
	
	playing = !playing;
	
}

function animationPlayer() {
	
	if( ! iterator.hasNext() ) {
			playing = false;
			$( "#playButton" ).html('Odtwarzaj');
			return;
	} else {
		
		setTimeout( function() {
			if( playing ) {
				oneForward();
				animationPlayer();
			}
		}, 100 );
	}
}

function parseInput() {
	
	var text = $( "#inputArea" ).val();
	inputDataReader = new InputDataReader( text );
	data = new DataRepository( inputDataReader.getPoints(), inputDataReader.getLines() );
	
	setTimeout( function() {
		data.addSnapshots( inputDataReader.getSnapshots() );
		updateLabel();
	}, 150 );
	
	iterator = new SnapshotsIterator( data );
	iterator.reset();
	updateLabel();
}


if(window.attachEvent) {   // just repeating last step
    window.attachEvent('onresize', function() {
        iterator.getPrevious();
        oneForward();
    });
}
else if(window.addEventListener) {
    window.addEventListener('resize', function() {
        iterator.getPrevious();
        oneForward();
    }, true);
}

