
function foo(a)
	print("foo",a);
	return coroutine.yield("foo_yield",2*a);
end

co = coroutine.create(
	 function(a,b)
		print("co-body",a,b);
		local v,r = foo(a+1);
		print("co-body",v,r);
		local v,r,s = coroutine.yield("co_yield",a+b,a-b);
		print("co-body",v,r,s);
	 end
)


