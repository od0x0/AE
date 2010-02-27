include AE/Core

AeVec2: cover from AEVec2{
	x,y: extern Float
}

AeVec3: cover from AEVec3{
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
AeVec4: cover from AEVec4{
	x,y,z,w: extern Float
}

AeVec2i: cover from AEVec2{
	x,y: extern Int
}

AeVec3i: cover from AEVec3{
	x,y,z: extern Int
}

AeVec4i: cover from AEVec4{
	x,y,z,w: extern Int
}


AeFace: cover from AEFace{
	v,n,t: extern UInt[3]
}

_AEMesh_count_struct_internal: cover{
	v,f,t,n: extern UInt
}

AeMesh: cover from AEMesh{
	count: extern _AEMesh_count_struct_internal
	v: extern AeVec3*
	t: extern AeVec2*
	n: extern AeVec3*
	aux: extern Pointer

	load: static extern(AEMeshLoad) func(String)->This*
	draw: static extern(AEMeshDrawRaw) func(This*)
	asVBO: static extern(AEMeshAsVBO) func(This*)->AeVBO*
	delete: static extern(AEMeshDelete) func(This*)
}

AeVBOVert: cover from AEVBOVert{
	t: extern AeVec2
	n,v: extern AeVec3
}

AeVBOVertWithoutNormal: cover from AEVBOVertWithoutNormal{
          t: extern AeVec2
          v: extern AeVec3
}

AeVBO: cover from AEVBO{
	vbo,ibo,vcount,icount,vallocated,iallocated: extern UInt
	indices: extern UInt*
	hasNormals: extern Char
	verts: extern AeVBOVert*

	load: static extern(AEVBOLoad) func(String,Int,Int)->This*
	add: static extern(AEVBOAdd) func(This*, AeVBOVert*)
	draw: static extern(AEVBODraw) func(This*)
	compile: static extern(AEVBOCompile) func(This*,UInt*)
	delete: static extern(AEVBODelete) func(This*)
}

AeObject: cover from AEObject extends AeVec3{
	rotation: extern AeVec3
	event: extern Func (This*,Int,Pointer)
	model: extern Pointer
	texture: extern UInt
	size,velocity,last: extern AeVec3
	mass: extern  Float
	isStatic: extern Char
	aux: extern Pointer

	new: static extern(AEObjectNew) func ~andAddToList(Func,Pointer)->This*
	collidesWith: extern(AEObjectsCollide) func(This*,This*)->Int
	defaultEvent: static extern(AEObjectDefaultEvent) func(This*,Int,Pointer)
	signalAll: static extern(AEObjectsSignal) func(Int,Pointer)
	physicsAll: static extern(AEObjectsPhysics) func(Float,AeVec3)
	delete: extern(AEObjectDelete) func(This*)
}

AeObjectEventInit: extern(AEObjectEventInit) Int
AeObjectEventRender: extern(AEObjectEventRender) Int
AeObjectEventCollide: extern(AEObjectEventCollide) Int
AeObjectEventRender: extern(AEObjectEventRender) Int
AeObjectEventRenderFromCompile: extern(AEObjectEventRenderFromCompile) Int

AeObjectDynamic: extern(AEObjectDynamic) Int
AeObjectStatic: extern(AEObjectStatic) Int
AeObjectCompiled: extern(AEObjectCompiled) Int

AeCameraType: cover from AECameraType extends AeVec3{
	rotation: extern AeVec3
	fov,near,far: extern Float
}

AeScreenType: cover from AEScreenType{
	w,h,r,g,b,a,stencil,depth,inFullscreen: extern Int
}
///////////////////////
AeCamera: extern(AECamera) AeCameraType
AeScreen: extern(AEScreen) AeScreenType
AeMouse: extern(AEMouse) AeVec2i
AeObjects: extern(AEObjects) AeObject**
AeObjectCount: extern(AEObjectCount) UInt
AeBlockKeyInput: extern(AEBlockKeyInput) Int
AeVBOAddAllocateFreq: extern(AEVBOAddAllocateFreq) UChar
AeTextureLoadFlags: extern(AETextureLoadFlags) UInt
////////////////////////

Ae: cover{
	init: static extern(AEInit) func(String,Int,Int)
	start: static extern(AEStart) func(Func (Float))
	quit: static extern(AEQuit) func
	refreshViewport: static extern(AERefreshViewport) func
	
	key: static extern(AEKey) func(Int)->Int
	mouseButton: static extern(AEMouseButton) func(Char)->Int
	
	textureLoad: static extern(AETextureLoad) func(String)->UInt
	textureBind: static extern(AETextureBind) func(UInt)
	textureDelete: static extern(AETextureDelete) func(UInt)

	angleTo: static extern(AEAngleTo) func(Float,Float,Float) -> AeVec3
	moveXZ: static extern(AEMoveXZ) func(Pointer,Float,Float,Float)
}
