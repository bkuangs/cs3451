#version 330 core

uniform vec2 iResolution;               /* screen resolution, value passed from CPU */
uniform float iTime;                    /* current time, value passed from CPU */
uniform int iFrame;                     /* current frame, value passed from CPU */
in vec2 fragCoord;                      /* fragment shader input: fragment coordinates, valued passed from vertex shader */
out vec4 fragColor;                     /* fragment shader output: fragment color, value passed to pixel processing for screen display */

const float M_PI = 3.1415926535;                        /* const value for PI */
const vec3 BG_COLOR = vec3(184, 243, 255) / 255.;       /* const value for background color */

//// This function converts from Polar Coordinates to Cartesian coordinates

vec2 polar2cart(float angle, float length)
{
    return vec2(cos(angle) * length, sin(angle) * length);
}

//// This is a sample function showing you how to check if a point is inside a triangle

bool inTriangle(vec2 p, vec2 p1, vec2 p2, vec2 p3)
{
    if(dot(cross(vec3(p2 - p1, 0), vec3(p - p1, 0)), cross(vec3(p2 - p1, 0), vec3(p3 - p1, 0))) >= 0. &&
        dot(cross(vec3(p3 - p2, 0), vec3(p - p2, 0)), cross(vec3(p3 - p2, 0), vec3(p1 - p2, 0))) >= 0. &&
        dot(cross(vec3(p1 - p3, 0), vec3(p - p3, 0)), cross(vec3(p1 - p3, 0), vec3(p2 - p3, 0))) >= 0.){
        return true;
    }
    return false;
}

//// This is a sample function showing you how to draw a rotated triangle 
//// Time is specified with iTime

vec3 drawTriangle(vec2 pos, vec2 center, vec3 color)
{
    vec2 p1 = polar2cart(iTime * 2, 160.) + center;
    vec2 p2 = polar2cart(iTime * 2 + 2. * M_PI / 3., 160.) + center;
    vec2 p3 = polar2cart(iTime * 2 + 4. * M_PI / 3., 160.) + center;
    if(inTriangle(pos, p1, p2, p3)){
        return color;
    }
    return vec3(0);
}


/////////////////////////////////////////////////////
//// Step 1 Function: Inside Circle
//// In this function, you will implement a function to checks if a point is inside a circle
//// The inputs include the point position, the circle's center and radius
//// The output is a bool indicating if the point is inside the circle (true) or not (false)
/////////////////////////////////////////////////////
//// Implementation hint: use dot(v,v) to calculate the squared length of a vector v
/////////////////////////////////////////////////////

bool inCircle(vec2 pos, vec2 center, float radius)
{
    /* your implementation starts */
    float distance = dot(vec2(pos - center), vec2(pos - center));

    if (distance <= radius * radius)
    {
        return true;
    }
    /* your implementation ends */
    
    return false;
}

//// This function calls the inCircle function you implemented above and returns the color of the circle
//// If the point is outside the circle, it returns a zero vector by default
vec3 drawCircle(vec2 pos, vec2 center, float radius, vec3 color)
{
    if(inCircle(pos, center, radius)){
        return color;
    }
    return vec3(0);
}

/////////////////////////////////////////////////////
//// Step 2 Function: Inside Rectangle
//// In this function, you will implement a function to checks if a point is inside a rectangle
//// The inputs include the point position, the left bottom corner and the right top corner of the rectangle
//// The output is a bool indicating if the point is inside the rectangle (true) or not (false)
/////////////////////////////////////////////////////
//// Implementation hint: use .x and .y to access the x and y components of a vec2 variable
/////////////////////////////////////////////////////

bool inRectangle(vec2 pos, vec2 leftBottom, vec2 rightTop)
{
    /* your implementation starts */
    if (
        (pos.x >= leftBottom.x && pos.x <= rightTop.x) &&
        (pos.y >= leftBottom.y && pos.y <= rightTop.y)
    )
    {
        return true;
    }
	
    /* your implementation ends */
    
    return false;
}

//// This function calls the inRectangle function you implemented above and returns the color of the rectangle
//// If the point is outside the rectangle, it returns a zero vector by default

vec3 drawRectangle(vec2 pos, vec2 leftBottom, vec2 rightTop, vec3 color)
{
    if(inRectangle(pos,leftBottom,rightTop)){
        return color;
    }
    return vec3(0);
}

bool inEllipse(vec2 pos, vec2 center, vec2 radius)
{
    vec2 d = pos - center;
    d = d / radius;

    return dot(d, d) <= 1.0;
}

bool inRotatedEllipse(vec2 pos, vec2 center, vec2 radius, float angle)
{
    vec2 d = pos - center;

    float c = cos(angle);
    float s = sin(angle);

    vec2 rotated = vec2(
        c * d.x + s * d.y,
       -s * d.x + c * d.y
    );

    rotated = rotated / radius;

    return dot(rotated, rotated) <= 1.0;
}

bool inCapsule(vec2 pos, vec2 a, vec2 b, float radius)
{
    vec2 pa = pos - a;
    vec2 ba = b - a;

    float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
    vec2 closest = a + h * ba;

    return dot(pos - closest, pos - closest) <= radius * radius;
}

// wave the arm
vec2 rotateAround(vec2 pos, vec2 pivot, float angle)
{
    vec2 d = pos - pivot;
    float c = cos(angle);
    float s = sin(angle);

    return pivot + vec2(c * d.x - s * d.y, s * d.x + c * d.y);
}

//// This function draws objects on the canvas by specifying a fragColor for each fragCoord

void mainImage(in vec2 fragCoord, out vec4 fragColor)
{
    //// Get the window center
    vec2 center = vec2(iResolution / 2.);

    /* Creative expression starts */

    //// background //// 
    float skyMix = clamp(fragCoord.y / iResolution.y, 0.0, 1.0);
    vec3 color = mix(vec3(0.72, 0.90, 1.0), vec3(0.90, 0.98, 1.0), skyMix);

    // sun
    if (inCircle(fragCoord, center + vec2(-300.0, 245.0), 58.0)) {
        color = vec3(1.0, 0.86, 0.25);
    }

    // tree trunks
    if (inRectangle(fragCoord, center + vec2(-390.0, -315.0), center + vec2(-370.0, -165.0))) {
        color = vec3(0.34, 0.18, 0.08);
    }
    if (inRectangle(fragCoord, center + vec2(350.0, -315.0), center + vec2(370.0, -155.0))) {
        color = vec3(0.34, 0.18, 0.08);
    }
    if (inRectangle(fragCoord, center + vec2(-300.0, -315.0), center + vec2(-282.0, -200.0))) {
        color = vec3(0.36, 0.20, 0.09);
    }

    // tree leaves
    if (inCircle(fragCoord, center + vec2(-380.0, -120.0), 75.0)) {
        color = vec3(0.08, 0.42, 0.15);
    }
    if (inCircle(fragCoord, center + vec2(-430.0, -165.0), 62.0)) {
        color = vec3(0.06, 0.35, 0.12);
    }
    if (inCircle(fragCoord, center + vec2(-330.0, -160.0), 62.0)) {
        color = vec3(0.10, 0.48, 0.18);
    }

    if (inCircle(fragCoord, center + vec2(360.0, -110.0), 82.0)) {
        color = vec3(0.07, 0.40, 0.14);
    }
    if (inCircle(fragCoord, center + vec2(300.0, -165.0), 66.0)) {
        color = vec3(0.05, 0.33, 0.12);
    }
    if (inCircle(fragCoord, center + vec2(420.0, -165.0), 66.0)) {
        color = vec3(0.11, 0.47, 0.18);
    }

    if (inCircle(fragCoord, center + vec2(-290.0, -185.0), 52.0)) {
        color = vec3(0.09, 0.38, 0.14);
    }
    if (inCircle(fragCoord, center + vec2(-325.0, -220.0), 44.0)) {
        color = vec3(0.06, 0.31, 0.11);
    }
    if (inCircle(fragCoord, center + vec2(-255.0, -220.0), 44.0)) {
        color = vec3(0.12, 0.46, 0.17);
    }

    //// define variables for arm wave ////
    vec2 leftArmPivot = center + vec2(-200.0, -50.0);
    float waveAngle = sin(iTime * 12.0) * 0.12;

    //// bamboo ////

    // stalk
    if (inCapsule(
        fragCoord,
        rotateAround(center + vec2(-355.0, -15.0), leftArmPivot, waveAngle),
        rotateAround(center + vec2(-175.0, 150.0), leftArmPivot, waveAngle),
        10.0
    )) {
        color = vec3(0.15, 0.65, 0.20);
    }

    // joints
    if (inCapsule(
        fragCoord,
        rotateAround(center + vec2(-315.0, 6.0), leftArmPivot, waveAngle),
        rotateAround(center + vec2(-335.0, 25.0), leftArmPivot, waveAngle),
        4.0
    )) {
        color = vec3(0.05, 0.35, 0.10);
    }
    if (inCapsule(
        fragCoord,
        rotateAround(center + vec2(-255.0, 57.0), leftArmPivot, waveAngle),
        rotateAround(center + vec2(-275.0, 78.0), leftArmPivot, waveAngle),
        4.0
    )) {
        color = vec3(0.05, 0.35, 0.10);
    }
    if (inCapsule(
        fragCoord,
        rotateAround(center + vec2(-210.0, 99.0), leftArmPivot, waveAngle),
        rotateAround(center + vec2(-230.0, 120.0), leftArmPivot, waveAngle),
        4.0
    )) {
        color = vec3(0.05, 0.35, 0.10);
    }

    // leaves
    if (inRotatedEllipse(
        fragCoord,
        rotateAround(center + vec2(-220.0, 120.0), leftArmPivot, waveAngle),
        vec2(38.0, 10.0),
        radians(25.0) + waveAngle
    )) {
        color = vec3(0.05, 0.45, 0.12);
    }
    if (inRotatedEllipse(
        fragCoord,
        rotateAround(center + vec2(-195.0, 167.0), leftArmPivot, waveAngle),
        vec2(34.0, 9.0),
        radians(-25.0) + waveAngle
    )) {
        color = vec3(0.05, 0.45, 0.12);
    }
    if (inRotatedEllipse(
        fragCoord,
        rotateAround(center + vec2(-177.0, 135.0), leftArmPivot, waveAngle),
        vec2(30.0, 8.0),
        radians(75.0) + waveAngle
    )) {
        color = vec3(0.05, 0.45, 0.12);
    }

    //// arms ////

    // left arm
    if (inCapsule(
        fragCoord,
        leftArmPivot,
        rotateAround(center + vec2(-250.0, 45.0), leftArmPivot, waveAngle),
        35.0
    )) {
        color = vec3(0.001);
    }
    // left fingers
    if (inCircle(fragCoord, rotateAround(center + vec2(-282.0, 54.0), leftArmPivot, waveAngle), 10.0)) {
        color = vec3(0.001);
    }
    if (inCircle(fragCoord, rotateAround(center + vec2(-275.0, 70.0), leftArmPivot, waveAngle), 10.0)) {
        color = vec3(0.001);
    }
    if (inCircle(fragCoord, rotateAround(center + vec2(-262.0, 78.0), leftArmPivot, waveAngle), 10.0)) {
        color = vec3(0.001);
    }
    if (inCircle(fragCoord, rotateAround(center + vec2(-244.0, 78.0), leftArmPivot, waveAngle), 10.0)) {
        color = vec3(0.001);
    }
    if (inCircle(fragCoord, rotateAround(center + vec2(-224.0, 68.0), leftArmPivot, waveAngle), 10.0)) {
        color = vec3(0.001);
    }

    // right arm
    if (inCapsule(
        fragCoord,
        center + vec2(200.0, -50.0),
        center + vec2(250.0, 45.0),
        35.0
    )) {
        color = vec3(0.001);
    }
    // right fingers
    if (inCircle(fragCoord, center + vec2(230.0, 74.0), 10.0)) {
        color = vec3(0.001);
    }
    if (inCircle(fragCoord, center + vec2(252.0, 80.0), 10.0)) {
        color = vec3(0.001);
    }
    if (inCircle(fragCoord, center + vec2(265.0, 76.0), 10.0)) {
        color = vec3(0.001);
    }
    if (inCircle(fragCoord, center + vec2(276.0, 68.0), 10.0)) {
        color = vec3(0.001);
    }
    if (inCircle(fragCoord, center + vec2(282.0, 55.0), 10.0)) {
        color = vec3(0.001);
    }

    //// legs ////

    // left leg
    if (inEllipse(fragCoord, center + vec2(-100.0, -350.0), vec2(50.0, 30.0))) {
        color = vec3(0.001);
    }
    // right leg
    if (inEllipse(fragCoord, center + vec2(100.0, -350.0), vec2(50.0, 30.0))) {
        color = vec3(0.001);
    }

    // body 
    if (inEllipse(fragCoord, center + vec2(0.0, -150.0), vec2(250.0, 225.0))) {
        color = vec3(1.0);
    }

    //// face ////

    // ears
    if (inCircle(fragCoord, center + vec2(-120.0, 230.0), 55.0)) {
        color = vec3(0.001);
    }
    if (inCircle(fragCoord, center + vec2(120.0, 230.0), 55.0)) {
        color = vec3(0.001);
    }

    // head
    if (inCircle(fragCoord, center + vec2(0.0, 100.0), 160.0)) {
        color = vec3(1.0);
    }

    // eye bags
    if (inEllipse(fragCoord, center + vec2(-60.0, 145.0), vec2(45.0, 60.0))) {
        color = vec3(0.001);
    }
    if (inEllipse(fragCoord, center + vec2(60.0, 145.0), vec2(45.0, 60.0))) {
        color = vec3(0.001);
    }

    // eyes
    if (inCircle(fragCoord, center + vec2(-60.0, 150.0), 14.0)) {
        color = vec3(1.0);
    }
    if (inCircle(fragCoord, center + vec2(60.0, 150.0), 14.0)) {
        color = vec3(1.0);
    }

    // pupils
    if (inCircle(fragCoord, center + vec2(-60.0, 150.0), 7.0)) {
        color = vec3(0.001);
    }
    if (inCircle(fragCoord, center + vec2(60.0, 150.0), 7.0)) {
        color = vec3(0.001);
    }

    //// mouth ////

    // black mouth oval
    if (inEllipse(fragCoord, center + vec2(0.0, 25.0), vec2(50.0, 35.0))) {
        color = vec3(0.001);
    }

    // cover upper part of mouth to make a smile
    if (inEllipse(fragCoord, center + vec2(0.0, 45.0), vec2(45.0, 25.0))) {
        color = vec3(1.0);
    }

    // tongue
    if (inEllipse(fragCoord, center + vec2(0.0, 5.0), vec2(30.0, 12.0))) {
        color = vec3(1.0, 0.35, 0.45);
    }

    // nose
    if (inCircle(fragCoord, center + vec2(0.0, 70.0), 22.0)) {
        color = vec3(0.001);
    }

    vec3 fragOutput = color;
    /* Creative expression end */

    //// By default we draw an animated triangle 
    // vec3 fragOutput = drawTriangle(fragCoord, center, vec3(1.0));
    
    //// Step 1: Uncomment this line to draw a circle
    // fragOutput = drawCircle(fragCoord, center, 250, vec3(1.0));

    
    //// Step 2: Uncomment this line to draw a rectangle 
    // fragOutput = drawRectangle(fragCoord, center - vec2(500, 50), center + vec2(500, 50), vec3(1.0));


    //// Step 3: Uncomment this line to draw an animated circle with a temporally varying radius controlled by a sine function
    // fragOutput = drawCircle(fragCoord, center, 150 + 50. * sin(iTime * 10), vec3(1.0));

    //// Step 4: Uncomment this line to draw a union of the rectangle and the animated circle you have drawn previously
    // fragOutput = drawRectangle(fragCoord, center - vec2(500, 50), center + vec2(500, 50), vec3(1.0)) + drawCircle(fragCoord, center, 150 + 50. * sin(iTime * 10), vec3(1.0));

    //// Set the fragment color to be the background color if it is zero
    if(fragOutput == vec3(0)){
        fragColor = vec4(BG_COLOR, 1.0);
    }
    //// Otherwise set the fragment color to be the color calculated in fragOutput
    else{
        fragColor = vec4(fragOutput, 1.0);
    }

    //// Step 5: Implement your customized scene by modifying the mainImage function
    //// Try to leverage what you have learned from Step 1 to 4 to define the shape and color of a new object in the fragment shader
    //// Notice how we put multiple objects together by adding their color values
}

void main()
{
    mainImage(fragCoord, fragColor);
}
