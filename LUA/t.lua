
function len(str)
 	local len = string.len(str);
 	local n = 0;
 	for i=1,len do
 		
 		local b = string.byte(str,i,i);
 		if b>128 then
 			i=i+1;
 			n=n+1
 			continue;
 		end
 		
 		n=n+1;
 	end
 	
 	return n;
 end
 
 print(len("abc"));
