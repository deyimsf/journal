package loader;

import java.net.URL;
import java.net.URLClassLoader;


public class MyClassLoaderURL extends URLClassLoader {

    public MyClassLoaderURL(URL[] urls) {
        super(urls);
    }


    @Override
    public Class loadClass(String className) throws ClassNotFoundException {
        if(!className.startsWith("com.cc")){
            return super.loadClass(className);
        }

        /**当包名是以com.cc开头的类，则破坏委托，先找自己路径下的，找不到再委托**/
        try{
            Class classZ =  this.findClass(className);
            if(classZ != null){
                 // 破坏委托，先找自己路径下的，找不到再委托(此时所有的类，包括jdk包中的，都是先在指定的路径中找，找不到再委托)
                return classZ;
            }
        }catch (ClassNotFoundException e){
            // swallow
        }

        return super.loadClass(className);
    }

}
