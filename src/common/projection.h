
#include "common.h"
#include "flatfield.h"




class Stitcher {

  static const std::string modname;

  Map wght;
  Map swght;
  int minx;
  int miny;

public:

  PointF2D origin;
  int isz;
  Shape ish;
  Shape osh;

  Stitcher()
    : isz(0), minx(0), miny(0)
  {}

  void prepare(PointF2D _origin, Shape _ish, int _isz, const std::deque<Map> & msks);

  inline void prepare(PointF2D _origin, Shape _ish, int _isz, const Map & msks) {
    std::deque<Map> msksdq(msks.size() ? 1 : 0, msks);
    prepare(_origin, _ish, _isz, msksdq);
  }

  void stitch(const std::deque<Map> & iarr, Map & oarr) const;

  Map resMask() const;

  static Shape outShape(PointF2D _origin, Shape _ish, int _isz) {
    if ( ! area(_ish) || ! _isz)
      return Shape();
    return Shape( _ish(0) + (_isz-1)*abs(_origin.y)
                , _ish(1) + (_isz-1)*abs(_origin.x));
  }

};



struct StitchRules {

  uint nofIn;
  Shape ish;
  Crop crp;                  ///< Crop input projection image
  Crop fcrp;                  ///< Crop final projection image
  Binn bnn;                  ///< binning factor
  float angle;                ///< Rotation angle.
  PointF2D origin1;            ///< Origin of the next image in the first stitch
  PointF2D origin2;            ///< Origin of the next image in the second stitch
  uint origin1size;
  uint origin2size;           ///< Nof images in the second stitch - needed only if it is requested (origin2)
  PointF2D originF;            ///< Origin of the flipped portion
  bool flipUsed;               ///< indicates if originF was given in options.
  std::deque<uint> splits;          ///< Split pooints to separate samples.
  uint edge;               ///< blur of mask and image edges.
  float sigma;             ///< sigma used in gaussian gap closure.

  StitchRules()
  : nofIn(0)
  , angle(0)
  , origin1size(1)
  , origin2size(1)
  , flipUsed(false)
  , edge(0)
  , sigma(0)
  {}

  Shape outShape() const {
    const Shape pish(ImageProc::outShape(angle, crp, bnn, ish));
    const Shape sish1(Stitcher::outShape(origin1, pish, origin1size));
    const Shape sish2(Stitcher::outShape(origin2, sish1, origin2size));
    return sish2;
  }

};





class ProcProj {

  static const std::string modname;
  static cl_program oclProgram;
  std::deque<FlatFieldProc> ffprocs;
  StitchRules strl;

  Stitcher   ster1_R, ster2_R, sterF_R;
  Stitcher & ster1,   ster2,   sterF;
  Map mskF; // shared
  ImageProc iproc; // own
  Map stitched, final; // own
  std::deque<Map> allIn, o1Stitch, o2Stitch;

  bool doGapsFill;
  CLkernel gaussCL;
  CLmem iomCL;
  CLmem maskCL_R;
  CLmem & maskCL;

  void prepareMask(Map & _gaps, bool bepicky);

  void initCL();

  void sub_proc( const Stitcher & ster, const std::deque<Map> & hiar, std::deque<Map> & oar
               , const ImagePath & interim_name, const std::string & suffix);

public:

  ProcProj( const StitchRules & _st
          , const std::deque<Map> & bgas, const std::deque<Map> & dfas
          , const std::deque<Map> & dgas, const std::deque<Map> & msas
          , const Path & saveMasks = Path());

  ProcProj(const ProcProj & other);


  bool process( std::deque<Map> & allInR, std::deque<Map> & res
              , const ImagePath & interim_name = ImagePath());

  Shape outShape() const {
    return strl.outShape();
  }

};
