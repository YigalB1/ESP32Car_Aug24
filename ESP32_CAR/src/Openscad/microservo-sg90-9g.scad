L = 22.8;
l = 12.6;


servo();

module servo(){
    //For easier layout
    rotate([0,0,90]) translate([-L/2,-l/2,0]) servoSG90();
} // of servo()

module servoSG90() {
//    L = 22.8;
//    l = 12.6;
    h = 22.8;
    plateL = 32.5;
    plateH = 2.7;
    plateHPos = 16;
    topCylinderH = 3.4;
    smallTopCylinderD = 4.5;
    
    axisH = 2;
    axisD = 4;
    
    screwHoleCenter=2;
    screwHoleD=2;
    holeSize=1;
    
    difference() {
        union() {
            color("LightBlue", 0.5) {
                // main part
                cube([L, l, h]);
                // support
                translate([-(plateL - L) / 2, 0, plateHPos]) {
                    cube([plateL, l, plateH]);
                }
                // top big cylinder
                translate([l/2,l/2,h]) {
                    cylinder(d=l, h=topCylinderH, $fn=180);
                }
                // top small cylinder
                translate([l, l/2, h]) { 
                    cylinder(d=smallTopCylinderD, h=topCylinderH, $fn=180);
                }
            }
            translate([l/2,l/2, h + topCylinderH]) {
                color("white") {
                    cylinder(d=axisD,h=axisH, $fn=180);
                }
            }
            


        } // of union()
        translate([-(plateL - L) / 2 + screwHoleCenter, l/2, plateHPos]) {
            cylinder(d=screwHoleD, h=10, $fn=180, center=true);
        }
        translate([-(plateL - L) / 2 - 1, l / 2 - holeSize / 2, plateHPos - 1]) {
            cube([3,holeSize,4]);
        }
        translate([plateL - (plateL - L) / 2 - screwHoleCenter, l / 2, plateHPos - 1]) {
            cylinder(d=screwHoleD, h=10, $fn=180, center=true);
        }
        translate([plateL- (plateL - L) / 2 - screwHoleCenter, l / 2 - holeSize / 2, plateHPos -1]) {
            cube([3,holeSize,4]);
        }
    } // of difference()
    // cylinders to make holes for screws
    translate([plateL-(plateL-L)/2-screwHoleCenter,l/2, plateHPos - 1]) {
            cylinder(d=screwHoleD, h=40, $fn=90, center=true);
        }
        
        
    translate([-(plateL-L)/2+screwHoleCenter,l/2,plateHPos]) {
            cylinder(d=screwHoleD, h=40, $fn=90, center=true);
        }
} //of servoSG90()

