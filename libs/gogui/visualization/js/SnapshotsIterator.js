
function SnapshotsIterator( dataRepositoryParam ) {
	
	this.dataRepository = dataRepositoryParam;
	
	this.numberOfSnapshots = this.dataRepository.getNumberOfSnapshots();
	this.currentSnapshot = -1;
}

SnapshotsIterator.prototype = {
	
	hasNext : function() {
		this.numberOfSnapshots = this.dataRepository.getNumberOfSnapshots();
		console.log( "Number of snapshots: " + this.numberOfSnapshots );
		return ( this.currentSnapshot < this.numberOfSnapshots - 1 ); 
	},
	
	hasPrevious : function() {
		return ( this.currentSnapshot > 0 );
	},
	
	getNext : function() {
		if( ! this.hasNext() ) {
			return null;
		} else {
			++this.currentSnapshot;
			console.log( "iterator: " + this.dataRepository.snapshots.length );
			return this.dataRepository.getSnapshot( this.currentSnapshot );
		}
	},
	
	getPrevious : function() {
		if( ! this.hasPrevious() ) {
			return null;
		} else {
			--this.currentSnapshot;
			return this.dataRepository.getSnapshot( this.currentSnapshot );
		}
	},
	
	getSnapshotIndex: function() {
		return this.currentSnapshot;
	},
	
	reset: function() {
		this.currentSnapshot = -1;
	}
};
