Usage (C++ only): ./slic3r [ OPTIONS ] [ file.stl ] [ file2.stl ] ...
---- Options
--center              Center the print around the given center
    (default: 100, 100).
--cut                 Cut model at the given Z.
		(default: 0)
--cut-grid            Cut model in XY plane into tiles of specified max size
		(default: 0,0)
--cut-x               Cut model at the given X.
		(default: 0)
--cut-y               Cut model at the given Y.
		(default: 0)
--export-3mf          Slice the model and export slices as 3MF.
		(default: 0)
--export-obj          Export the model as OBJ.
		(default: 0)
--export-pov          Export the model as POV-Ray definition.
		(default: 0)
--export-svg          Slice the model and export slices as SVG.
		(default: 0)
--gui                 Start the Slic3r GUI.
		(default: 0)
--help                Show this help.
		(default: 0)
--info                Write information about the model to the console.
		(default: 0)
--load                Load configuration from the specified file. It can be used more than once to load options from multiple files.
		(default: )
--output              The file where the output will be written (if not specified, it will be based on the input file).
		(default: )
--rotate              Rotation angle around the Z axis in degrees (0-360)
		(default: 0)
--rotate-x            Rotation angle around the X axis in degrees (0-360)
		(default: 0)
--rotate-y            Rotation angle around the Y axis in degrees (0-360)
		(default: 0)
--save                Save configuration to the specified file.
		(default: )
--scale               Scaling factor (default: 1).
		(default: 1)
--scale-to-fit        Scale to fit the given volume.
		(default: 0,0,0)
--slice               Slice the model and export gcode.
		(default: 0)
