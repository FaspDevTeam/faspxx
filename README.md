# Fast Auxiliary Space Preconditioning (FASP++) in CPP: README

The FASP++ is a C++ package designed for developing parallel iterative solvers
and preconditioners for PDEs and systems of PDEs. The main components of the
package are standard Krylov methods, algebraic multigrid methods, geometric
multigrid methods, Schwarz methods, incomplete factorization methods, etc.

## Install
There is a top level cmake for configuration and building of the FASP++ library 
and the test programs suite. You can use a cmake-style approach to compile 
the package; see https://cmake.org on how to use cmake for your own operating 
system. The typical command for compiling FASP++ is:

Config the environment for building with cmake:
```bash
  > mkdir Build; cd Build; cmake ..
```

After succesfully configing, just make the library as well as examples:
```bash
  > make  # To compile the FASP++ lib only; Do not install
```

To make the library and install it, run:
```bash
  > make install  # To compile and install the FASP++ lib
```

## License
This software is free software distributed under the Lesser General Public
License or LGPL, version 3.0 or any later versions. This software distributed
in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with FASP++. If not, see <http://www.gnu.org/licenses/>.
