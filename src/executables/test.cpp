

#include "../common/common.h"
#include "../common/poptmx.h"


using namespace std;


/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  Path in_name;               ///< Name of the input file.
  Path out_name;              ///< Name of the output file.
  uint rad;                   ///< radius
  float coeff;                ///< coefficient
  Crop crp;                  ///< Crop input projection image
  Binn bnn;                  ///< binning factor
  int arf;
  bool saveInt;
  bool beverbose;       ///< Be verbose flag
  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[])
  : out_name("proc-<input>")
  , rad(0)
  , coeff(1.0)
  , arf(1)
  , saveInt(false)
  , beverbose(false)
{

  poptmx::OptionTable table("Test process.", "Just to test something.");

  table
    .add(poptmx::NOTE, "ARGUMENTS:")
    .add(poptmx::ARGUMENT, &in_name, "input", "Input image.", "The image to work on")
    .add(poptmx::ARGUMENT, &out_name, "output", "Output image.", "The image to store results", out_name)

    .add(poptmx::NOTE, "OPTIONS:")
    .add(poptmx::OPTION, &rad, 'r', "radius", "Some radius.", "Long description of radius.")
    .add(poptmx::OPTION, &coeff, 'c', "coefficient", "Some number", "")
    .add(poptmx::OPTION, &arf, 'F', "anarg", "description", "Long description")
    .add(poptmx::OPTION, &crp, 0, "crop", "image crop", "")
    .add(poptmx::OPTION, &bnn, 0, "binn", "image binn", "")
    .add(poptmx::OPTION, &saveInt, 'i', "int", "Output image(s) as integer.", IntOptionDesc)
    .add_standard_options(&beverbose)
    .add(poptmx::MAN, "SEE ALSO:", SeeAlsoList);

  if ( ! table.parse(argc,argv) )
    exit(0);
  if ( ! table.count() ) {
    table.usage();
    exit(0);
  }
  command = table.name();

  // <input> : one required argument.
  if ( ! table.count(&in_name) )
    exit_on_error(command, "Missing required argument: "+table.desc(&in_name)+".");
  // <output> : one more argument may or may not exist
  if ( ! table.count(&out_name) )
    out_name = upgrade(in_name, "res-");

}




/// \MAIN{ct}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv) ;

  Map arr;
  ReadImage(args.in_name, arr);
  const Shape sh(arr.shape());
  const size_t sz(arr.size());

  // Note the order: z, y, x; in Shape's they are Shape(0), Shape(1) and Shape(2)
  for (ArrIndex y = 0 ; y < sh(0) ; y++)
    for (ArrIndex x = 0 ; x < sh(1) ; x++)
      arr(y,x) += args.rad;

  arr *= args.coeff;
  crop(arr, args.crp);
  binn(arr, args.bnn);

  Line firstHoriz(sh(1)); // here instance
  firstHoriz = arr(0,all);\
  cout << firstHoriz.isStorageContiguous() << "\n";
  float * arrPointerC = firstHoriz.data() ;
  for ( int idx = 0 ; idx < firstHoriz.size() ; idx++ )
    *(arrPointerC++) = 2.0; // this is fine as instance has data contiguous

  Line firstVertRef = arr(all,0); // here reference
  cout << firstVertRef.isStorageContiguous() << "\n";
  float * arrPointerR = arr.data() ; // don't use this as reference's data may or may not be contiguous
  firstVertRef = 2.0; // fine

  Map arr1(sh(0)/2, sh(1));
  arr1 = arr( blitz::Range(0,sh(0)/2-1), all ); // Don't do this as Range sometimes segfaults

  SaveImage(args.out_name, arr1, args.saveInt);
  exit(0);

}
