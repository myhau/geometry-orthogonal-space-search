
function DataRepository(pointsParam, linesParam) {
	
	this.ready = false;
	
	this.xMin = null;
	this.yMin = null;
	this.xMax = null;
	this.yMax = null;
	
	this.points = pointsParam;
	this.lines = linesParam;
	
	this.numberOfSnapshots = 0;
	this.numberOfPoints = this.points.length;
	this.numberOfLines = this.lines.length;
	
	this.snapshots = [];
	
	this.ready = true;
	
	for(var i = 0;i < this.points.length ; i++){
	
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
};

DataRepository.prototype = {
	
	constructor: DataRepository,
	
	addSnapshots : function( snapshotsToAdd ) {
		
		while( ! this.ready ) {}
		this.ready = false;
		
		console.log( snapshotsToAdd );
		
		for( var i = 0; i < snapshotsToAdd.length; ++i ) {
			this.snapshots.push( snapshotsToAdd[i] );
			++this.numberOfSnapshots;
		}
		this.ready = true;
	},
	
	
	getNumberOfSnapshots : function() {
		return this.numberOfSnapshots;
	},
	
	getNumberOfLines : function() {
		return this.numberOfLines;
	},
	
	getNumberOfPoints: function() {
		return this.numberOfPoints;
	},
	
	getPointByIndex : function( index ) {
		if( index >= 0 && index < this.points.length ) {
			return this.points[index];
		} else {
			return null;
		}
	},
	
	getLineByIndex : function( index ) {
		if( index >= 0 && index < this.lines.length ) {
			return this.lines[index];
		} else {
			return null;
		}
	},
	
	getSnapshot : function( index ) {
		if( index >= 0 && index < this.snapshots.length ) {
			return this.snapshots[index];
		} else {
			return null;
		}
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
};

