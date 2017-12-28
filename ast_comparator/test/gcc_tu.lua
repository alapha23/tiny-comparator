local function add(i,T)
	local a=T[i]
	if a==nil then a={} T[i]=a end
	return a
end

local function store(L,T)
	if L then
		local ok,_,i,k=string.find(L,"^@(%d+) +(%S+)")
		assert(ok,"bad line")
		i=tonumber(i)
		local t=add(i,T)
		t.text=L
		t.kind=k
		for k,v in string.gfind(L,"(%w+): +(%S+)") do
			local _,_,j=string.find(v,"^@(%d+)$")
			if j then j=tonumber(j) v=add(j,T) assert(v,j) end
			t[k]=v
		end
	end
end

local function load(f)
	local T={}
	local L
	while true do
		local l=f:read()
		if l==nil then break end
		if string.find(l,"^@")  then
			store(L,T)
			L=l
		elseif string.find(l,"^ ") then
			L=L..l
		else
			error"bad line"
		end
	end
	store(L,T)
	return T
end

local T=load(io.stdin)

W={
var_decl = true,
-- type_decl = true,
-- function_decl = true,
}

for i,t in T do
	if W[t.kind] then
		if t.name then
			print(i,t.name.strg,t.type.kind,t.srcp)
		else
			print(i,nil,t.type.kind,t.srcp)
		end
	end
end
