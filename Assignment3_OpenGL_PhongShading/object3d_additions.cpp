// ====================================================================
// Add the following pure virtual function to Object3D and define it
// in each of its subclasses. The paint routine is responsible for
// making the OpenGL calls to draw the object to the screen.
// ==================================================================== #include
virtual void paint(void) = 0;
// ====================================================================
// Below are some OpenGL commands that will be essential for
// implementing the paint routines.
// // Use the glPushMatrix() and glPopMatrix() commands to save the
// current transformation to the stack or restore a transformation
// matrix from the top of the stack.
// ====================================================================
glPushMatrix();
glPopMatrix();
// ====================================================================
// These routines are used to translate, rotate, or scale an object
// The indicated transformation will modify the current matrix, and
// will thus affect all subsequent OpenGL calls.
// // glTranslate translates by (x,y,z)
// glRotate rotates by 'angle' degrees about axis (x,y,z)
// glScale scales by (x,y,z)
// // Data type can be either float or double, depending on whether
// the f or d is used in the function name
// ====================================================================
glTranslate{fd}(x, y, z);
glRotate{fd}(angle, x, y, z);
glScale{fd}(x, y, z);
// ====================================================================
// The parameters to glVertex can be either a listing or an array
// of floats or doubles, depending on how the routine is called
// ====================================================================
glVertex3f(float x, float y, float z);
// List of floats glVertex3d(double x, double y, double z);
// List of doubles glVertex3fv(float *arr);
// Array of floats glVertex3dv(double *arr);
// Array of doubles
// ====================================================================
// glMultMatrix{fd} can be used to multiply the current matrix by
// an arbitrary matrix. We've added a glGet() routine to the Matrix
// class to construct a matrix with the appropriate structure.
// // OpenGL matrices created with this routine should be deleted
// when they are no longer needed.
// ====================================================================
GLfloat *glMatrix = matrix.glGet();
glMultMatrixf(glMatrix);
delete[] glMatrix;
// ====================================================================
// Geometry is constructed with a series of calls to glVertex,
// sandwiched between glBegin() and glEnd() statements.
// ====================================================================
glBegin(GL_TRIANGLES);
glVertex3f(x0, y0, z0);
glVertex3f(x1, y1, z1);
glVertex3f(x2, y2, z2);
glEnd(); 
// ==================================================================== 
// The parameter passed to the glBegin() command changes how the 
// following vertices are interpreted. 
// // GL_POINTS: Each vertex will be drawn as a single pixel 
// GL_TRIANGLES: Each set of three vertices will be drawn as a triangle 
// GL_QUADS: Each set of four vertices will be drawn as a quadrilateral 
// GL_LINES: Each pair of vertices will be drawn as a line segment 
// // To change the normal vector, use glNormal3{fd}[v](). This routine 
// can take a variety of different parameters, just like glVertex(). 
// ==================================================================== 
glNormal3f(float x, float y, float z); 
// List of floats glNormal3d(double x, double y, double z); 
// List of doubles glNormal3fv(float *arr); 
// Array of floats glNormal3dv(double *arr); 
// Array of doubles 
// ==================================================================== 
// Note that some statements, like glVertex() and glNormal() should 
// always appear between glBegin() and glEnd() statements. Other 
// commands, like glPushMatrix() or glTranslate(), should not appear 
// between glBegin() and glEnd(). 
// ====================================================================
