Font=nil
Image=nil
Frame=0

UI={}

function UI:renderBox(rect,radius)
	gl.Rect(rect.x, rect.y, rect.x+rect.w, rect.y+rect.h)
end

amountx=0
amountz=0
Time=0
FPS=0

function perframe(step)
	AEUIRenderMode(1)
	AEText(FPS,Font,400,250)
	gl.Translate(amountx,amountz,0)
	if Time>5 then
		Time=0
		FPS=1/step
	end
	Time=Time+step
	AEPollInput()
	if AEKey('w') then
		amountz = amountz +step*100
		--os.exit(0)
	end
	if AEKey('s') then
		amountz = amountz -step*100
	end
	if AEKey('d') then
		amountx = amountx +step*100
		--os.exit(0)
	end
	if AEKey('a') then
		amountx = amountx-step*100
	end
	gl.Color(0.2,0.2,0.2)
	local rect={}
	rect.x=400-20
	rect.y=400-20
	rect.w=40
	rect.h=40
	--rect={x:200,y:200,w:100,h:100}
	UI:renderBox(rect,0)
	--AEImageBlit(AENULL,200,125,Image)
	AEUIRenderMode(0)
end

function main()
	--[[for k,v in pairs(_G) do
		print(k)
	end]]
	AEInit("Hello from the World of Lua",800,500)
	AEFontInit()
	--"Data/ARIAL.TTF"
	Font=AEFontNew("/Library/Fonts/Andale Mono.ttf",10)
	gl.ClearColor(1,1,1,1)
	Image=AEImageFromFile("Data/Texture.png");
	AEStart("perframe")
end

main()