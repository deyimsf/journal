package loader;

import sun.misc.URLClassPath;

import java.net.MalformedURLException;
import java.net.URL;


public class Demo2 {

    public static void main(String[] args) throws MalformedURLException, ClassNotFoundException {

        /**********加载某个jar包下的类***********/
        URL[] classpathA  = new URL[] { new URL("file:/My/class/classes/")};
        URL[] classpathB  = new URL[] {new URL("file:/My/class/abc.jar")};
        MyClassLoaderURL loaderA = new MyClassLoaderURL(classpathA);
        MyClassLoaderURL loaderB = new MyClassLoaderURL(classpathB);

        Class classA = loaderA.loadClass("com.cc.Person");
        Class classB = loaderB.loadClass("com.cc.Person");

        System.out.println(classA.hashCode());
        System.out.println(classB.hashCode());



        /*******************资源查找********************/
        URLClassPath uRLClassPathA = new URLClassPath(classpathA);
        URLClassPath uRLClassPathB = new URLClassPath(classpathB);
        System.out.println(uRLClassPathA.findResource("com/cc/Person.class",false));
        System.out.println(uRLClassPathB.findResource("com/cc/Person.class",false));

    }

}

