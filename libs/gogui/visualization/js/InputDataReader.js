

function InputDataReader( input ) {
	
	this.done = false;
	
	this.snapshots = [];
	this.lines = [];
	this.points = [];
	
	this.xMin = null;
	this.yMin = null;
	this.xMax = null;
	this.yMax = null;
	
	try {
		var json = jQuery.parseJSON( input );
	}
	catch( err ) {
		console.log( "Received input is not a proper JSON object" );
	}

	this.xMin = json.points[0].x;
	this.yMin = json.points[0].y;
	this.xMax = json.points[0].x;
	this.yMax = json.points[0].y;

	var scale = json.scale;
	for( var i = 0; i < json.history.length; ++i ) {
		var s = new Snapshot();
		s.setPoints( json.history[i].points );
		s.setLines( json.history[i].lines );
		this.snapshots.push(s);
	}
	
	for( var i = 0; i < json.points.length; ++i ) {
		this.points.push( json.points[i] );
		if( this.points[i].x > this.xMax ) {
			this.xMax = this.points[i].x;
		}
		if( this.points[i].x < this.xMin ) {
			this.xMin = this.points[i].x;
		}
		if( this.points[i].y > this.yMax ) {
			this.yMax = this.points[i].y;
		}
		if( this.points[i].y < this.yMin ) {
			this.yMin = this.points[i].y;
		}
	}
	
	for( var i = 0; i < json.lines.length; ++i ) {
		this.lines.push(json.lines[i]);
	}
	
	console.log( this.snapshots );
		
	this.done = true;
		
}

InputDataReader.prototype = {
	
	constructor : InputDataReader,
	
	getLines : function() {
		return this.lines;
	},
	
	getPoints : function() {
		return this.points;
	},
	
	getSnapshots : function() { 
		return this.snapshots;
	},
	
	getXMin: function() {
		return this.xMin;
	},
	
	getXMax: function() {
		return this.xMax;
	},
	
	getYMin: function() {
		return this.yMin;
	},
	
	getYMax: function() {
		return this.yMax;
	}
	
}
