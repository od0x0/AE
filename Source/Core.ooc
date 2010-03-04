include Core

Vec2: cover from AEVec2{
	x,y: extern Float
}

Vec3: cover from AEVec3{
	x,y,z: extern Float
	new: static func(.x,.y,.z)->This{
		this: This
		this x=x
		this y=y
		this z=z
		this
	}
	new: static func ~fromScalar(value: Float)->This{
		new(value, value, value)
	}
	new: static func ~blank->This{
		new(0)
	}
}
Vec4: cover from AEVec4{
	x,y,z,w: extern Float
}

Vec2i: cover from AEVec2{
	x,y: extern Int
}

Vec3i: cover from AEVec3{
	x,y,z: extern Int
}

Vec4i: cover from AEVec4{
	x,y,z,w: extern Int
}


Face: cover from AEFace{
	v,n,t: extern UInt[3]
}

_AEMesh_count_struct_internal: cover{
	v,f,t,n: extern UInt
}

Mesh: cover from AEMesh{
	count: extern _AEMesh_count_struct_internal
	v: extern Vec3*
	t: extern Vec2*
	n: extern Vec3*
	aux: extern Pointer

	load: static extern(AEMeshLoad) func(String)->This*
	draw: static extern(AEMeshDrawRaw) func(This*)
	asVBO: static extern(AEMeshAsVBO) func(This*)->VBO*
	delete: static extern(AEMeshDelete) func(This*)
}

VBOVert: cover from AEVBOVert{
	t: extern Vec2
	n,v: extern Vec3
}

VBOVertWithoutNormal: cover from AEVBOVertWithoutNormal{
          t: extern Vec2
          v: extern Vec3
}

VBO: cover from AEVBO{
	vbo,ibo,vcount,icount,vallocated,iallocated: extern UInt
	indices: extern UInt*
	hasNormals: extern Char
	verts: extern VBOVert*

	load: static extern(AEVBOLoad) func(String,Int,Int)->This*
	add: static extern(AEVBOAdd) func(This*, VBOVert*)
	draw: static extern(AEVBODraw) func(This*)
	compile: static extern(AEVBOCompile) func(This*,UInt*)
	delete: static extern(AEVBODelete) func(This*)
}

Object_: cover from AEObject extends Vec3{
	rotation: extern Vec3
	event: extern Func (This*,Int,Pointer)
	model: extern Pointer
	texture: extern UInt
	size,velocity,last: extern Vec3
	mass: extern  Float
	isStatic: extern Char
	aux: extern Pointer

	new: static extern(AEObjectNew) func ~andAddToList(Func,Pointer)->This*
	collidesWith: extern(AEObjectsCollide) func(This*,This*)->Int
	defaultEvent: static extern(AEObjectDefaultEvent) func(This*,Int,Pointer)
	signalAll: static extern(AEObjectsSignal) func(Int,Pointer)
	physicsAll: static extern(AEObjectsPhysics) func(Float,Vec3)
	delete: extern(AEObjectDelete) func(This*)
}

ObjectEventInit: extern(AEObjectEventInit) Int
ObjectEventRender: extern(AEObjectEventRender) Int
ObjectEventCollide: extern(AEObjectEventCollide) Int
ObjectEventRender: extern(AEObjectEventRender) Int
ObjectEventRenderFromCompile: extern(AEObjectEventRenderFromCompile) Int

ObjectDynamic: extern(AEObjectDynamic) Int
ObjectStatic: extern(AEObjectStatic) Int
ObjectCompiled: extern(AEObjectCompiled) Int

CameraType: cover from AECameraType extends Vec3{
	rotation: extern Vec3
	fov,near,far: extern Float
}

ScreenType: cover from AEScreenType{
	w,h,r,g,b,a,stencil,depth,inFullscreen: extern Int
}
///////////////////////
Camera: extern(AECamera) CameraType
Screen: extern(AEScreen) ScreenType
Mouse: extern(AEMouse) Vec2i
Objects: extern(AEObjects) Object_**
ObjectCount: extern(AEObjectCount) UInt
BlockKeyInput: extern(AEBlockKeyInput) Int
VBOAddAllocateFreq: extern(AEVBOAddAllocateFreq) UChar
TextureLoadFlags: extern(AETextureLoadFlags) UInt
////////////////////////
/*
init: static extern(AEInit) func(String,Int,Int)
start: static extern(AEStart) func(Func (Float))
quit: static extern(AEQuit) func
refreshViewport: static extern(AERefreshViewport) func
	
key: static extern(AEKey) func(Int)->Int
mouseButton: static extern(AEMouseButton) func(Char)->Int

textureLoad: static extern(AETextureLoad) func(String)->UInt
textureBind: static extern(AETextureBind) func(UInt)
textureDelete: static extern(AETextureDelete) func(UInt)

angleTo: static extern(AEAngleTo) func(Float,Float,Float) -> Vec3
moveXZ: static extern(AEMoveXZ) func(Pointer,Float,Float,Float)

*/
init: extern(AEInit) func(String,Int,Int)
start: extern(AEStart) func(Func (Float))
quit: extern(AEQuit) func
refreshViewport: extern(AERefreshViewport) func
	
key: extern(AEKey) func(Int)->Int
mouseButton: extern(AEMouseButton) func(Char)->Int
pollInput: extern(AEPollInput) func

textureLoad: extern(AETextureLoad) func(String)->UInt
textureBind: static extern(AETextureBind) func(UInt)
textureDelete: static extern(AETextureDelete) func(UInt)

angleTo: extern(AEAngleTo) func(Float,Float,Float) -> Vec3
moveXZ: extern(AEMoveXZ) func(Pointer,Float,Float,Float)
