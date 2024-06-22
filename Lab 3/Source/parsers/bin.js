/**
 * Selects the 'bin' extension.
 */
registerFileType((fileExt, filePath, data) => {
	// Check for bin
	return (fileExt === 'bin' || fileExt === 'res');
});



/**
 * Parser for obj files.
 * This parser just demonstrates the very basics of the parsing commands.
 */
registerParser(() => {
	read(1);
	addRow('Mode', getStringValue(), "Mode of data: S: SIFT, H: Histograms, C: Correllograms, O: ORB");
	const mode = getStringValue();
	read(4);
	const count = getNumberValue();
	addRow("Total elements", count)
	
	switch (mode) {
		case 'S':
		case 'O':
			readRowWithDetails('Size of descriptors', () => {
				for (let i = 0; i < count; i++) {
					read(4);
					addRow('Count', getNumberValue(), "Count of keypoint descriptors");
				}
				return {
					value: '' + count + ' descriptors',
				};
			});
			break;
	}
	read(4);
	addRow('Rows', getNumberValue(), "Number of rows in data matrix");
	read(4);
	addRow('Cols', getNumberValue(), "Number of columns in data matrix");
	read(4);
	addRow('Type ID', getNumberValue())
});