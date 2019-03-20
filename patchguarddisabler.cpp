#include "main.h"
#include "patchguarddisabler.h"

PatchTable ntoskrnlTable[WVERSIONS]={
  {
    VISTA,5,
    {
      {
        30,23,0,0,0,
        //"         x   x   x   ?   x   x   x   ?   ?   ?   ?   x   x   x   x   ?   x   ?   ?   ?   ?   x   ?   x   x   x   x   x   ?   x"
        (PBYTE)&"\x48\x83\xEC\xFF\x48\x8B\x05\xFF\xFF\xFF\xFF\x48\x85\xC0\x75\xFF\xB8\xFF\xFF\xFF\xFF\xEB\xFF\xFF\xD0\x48\x83\xC4\xFF\xC3",
        (PBYTE)&"\x8B\x01\x83\xF8\x08\x75\x0A\xC7\x41\x04\x01\x00\x00\x00\x31\xC0\xC3\xB8\x04\x00\x00\xC0\xC3",
        NULL
      },
      {
        11,2,0,0,66,
        //"         x   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x8B\xF8\xEB\x0F\x8B\x9C\x24\xE8\x00\x00\x00",
        (PBYTE)&"\x33\xFF",
        NULL
      },
      {
        17,3,0,0,0,
        //"         x   x   x   x   x   x   x   x   ?   ?   ?   ?   x   x   x   x   x"
        (PBYTE)&"\x48\x83\xEC\x28\x33\xC0\x38\x05\xFF\xFF\xFF\xFF\x74\x13\x4C\x8B\x0D",
        (PBYTE)&"\x33\xC0\xC3",
        NULL
      },
      {
        16,2,0,40,0,
        //"         x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x44\x89\x44\x24\x18\x89\x54\x24\x10\x89\x4C\x24\x08\x53\x55\x56",
        (PBYTE)&"\x90\x90",
        NULL
      },
      {
        22,2,0,20,0,
        //"         x   x   x   x   x   x   x   x   x   x   x   x   ?   ?   ?   ?   x   x   x   x   x   x"
        (PBYTE)&"\x74\x04\x3B\xCF\x74\xC6\x8B\xD1\x41\x8B\xC9\xE8\xFF\xFF\xFF\xFF\x84\xC0\x75\x02\x33\xDB",
        (PBYTE)&"\x90\x90",
        NULL
      },
    }
  },{
    WIN7,4,
    {
      {
        30,23,0,0,0,
        //"         x   x   x   ?   x   x   x   ?   ?   ?   ?   x   x   x   x   ?   x   ?   ?   ?   ?   x   ?   x   x   x   x   x   ?   x"
        (PBYTE)&"\x48\x83\xEC\xFF\x48\x8B\x05\xFF\xFF\xFF\xFF\x48\x85\xC0\x75\xFF\xB8\xFF\xFF\xFF\xFF\xEB\xFF\xFF\xD0\x48\x83\xC4\xFF\xC3",
        (PBYTE)&"\x8B\x01\x83\xF8\x08\x75\x0A\xC7\x41\x04\x01\x00\x00\x00\x31\xC0\xC3\xB8\x04\x00\x00\xC0\xC3",
        NULL
      },
      {
        24,1,0,32,0,
        //"         x   x   x   x   x   x   x   x   x   x   x   x   x   x   ?   ?   ?   ?   x   x   x   x   x   x"
        (PBYTE)&"\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x33\xDB\x38\x1D\xFF\xFF\xFF\xFF\x0F\x85\x94\x00\x00\x00",
        (PBYTE)&"\x00",
        NULL
      },
      {
        32,2,0,40,0,
        //"         x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x44\x89\x44\x24\x18\x89\x54\x24\x10\x89\x4C\x24\x08\x53\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x58\x0F\x00\x00",
        (PBYTE)&"\x90\x90",
        NULL
      },
      {
        22,2,0,20,0,
        //"         x   x   x   x   x   x   x   x   x   x   x   x   ?   ?   ?   ?   x   x   x   x   x   x"
        (PBYTE)&"\x74\x04\x3B\xCF\x74\xC6\x8B\xD1\x41\x8B\xC9\xE8\xFF\xFF\xFF\xFF\x84\xC0\x75\x02\x33\xDB",
        (PBYTE)&"\x90\x90",
        NULL
      },
    }
  },{
    WIN8,3,
    {
      {
        17,3,0,0,0,
        //"         x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x48\x8B\xC4\x4C\x89\x48\x20\x44\x89\x40\x18\x89\x50\x10\x89\x48\x08",
        (PBYTE)&"\xB0\x01\xC3",
        NULL
      },
      {
        16,2,0,0,0,
        //"         x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x8B\xF8\x4C\x3B\x7C\x24\x70\x74\x0F\x4D\x85\xFF\x74\x0A\x33\xD2",
        (PBYTE)&"\x33\xC0",
        NULL
      },
      {
        23,3,0,0,0,
        //"         x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x20\x48\x8B\x05",
        (PBYTE)&"\x33\xC0\xC3",
        NULL
      },
    }
  },{
    WIN81,4,
    {
      {//sub of KiFilterFiberContext
        17,3,0,0,0,
        //"         x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x48\x8B\xC4\x4C\x89\x48\x20\x44\x89\x40\x18\x89\x50\x10\x89\x48\x08",
        (PBYTE)&"\xB0\x01\xC3",
        NULL
      },
      {//MiValidateSectionCreate
        42,13,8,35,101,
        //"         x   x   x   ?   ?   ?   ?   x   x   x   ?   x   x   x   x   ?   ?   ?   ?   x   x   x   x   x   x   x   ?   ?   ?   ?   x   ?   ?   ?   ?   x   x   x   x   x   x   x"
        (PBYTE)&"\x8B\x84\x24\xFF\xFF\xFF\xFF\x89\x44\x24\xFF\x4C\x8B\x8C\x24\xFF\xFF\xFF\xFF\x49\x8B\xD5\x48\x8B\x8C\x24\xFF\xFF\xFF\xFF\xE8\xFF\xFF\xFF\xFF\x8B\xF8\x3D\x2D\x01\x00\x00",
        (PBYTE)&"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90",
        (PBYTE)&"\x90\x90\x90\x90\x90\x90\x90\x90"
      },
      {//SeCompareSigningLevelsForAuditableProcess
        18,6,0,0,0,
        //"         x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x8B\xDA\x48\x8B\xF9\x41\x8A\xD1",
        (PBYTE)&"\xB8\x01\x00\x00\x00\xC3",
        NULL
      },
      {//SeValidateImageData
        21,3,0,0,0,
        //"         x   x   x   ?   ?   ?   ?   x   x   x   x   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x4C\x8B\x15\xFF\xFF\xFF\xFF\x4D\x85\xD2\x74\x03\x49\xFF\xE2\xB8\x28\x04\x00\xC0\xC3",
        (PBYTE)&"\x33\xC0\xC3",
        NULL
      },
    }
  },{
    WIN10,4,
    {
      {//sub of KiFilterFiberContext + XX -> call
        17,3,0,0,0,
        //"         x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x48\x8B\xC4\x4C\x89\x48\x20\x44\x89\x40\x18\x89\x50\x10\x89\x48\x08",
        (PBYTE)&"\xB0\x01\xC3",
        NULL
      },
      {//MiValidateSectionCreate
        35,38,9,27,93,
        //"         x   x   x   ?   ?   ?   ?   x   x   x   ?   x   x   x   ?   x   x   ?   x   x   x   ?   x   ?   ?   ?   ?   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x8B\x84\x24\xFF\xFF\xFF\xFF\x89\x44\x24\xFF\x4C\x8B\x4D\xFF\x49\x8B\xFF\x48\x8B\x4D\xFF\xE8\xFF\xFF\xFF\xFF\x44\x8B\xF8\x3D\x2D\x01\x00\x00",
        (PBYTE)&"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90",
        (PBYTE)&"\x90\x90\x90\x90\x90\x90\x90\x90\x90"
      },
      {//SeCompareSigningLevelsForAuditableProcess
        18,6,0,0,0,
        //"         x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x8B\xDA\x48\x8B\xF9\x41\x8A\xD1",
        (PBYTE)&"\xB8\x01\x00\x00\x00\xC3",
        NULL
      },
      {//SeValidateImageData
        21,3,0,0,0,
        //"         x   x   x   ?   ?   ?   ?   x   x   x   x   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x4C\x8B\x15\xFF\xFF\xFF\xFF\x4D\x85\xD2\x74\x03\x49\xFF\xE2\xB8\x28\x04\x00\xC0\xC3",
        (PBYTE)&"\x33\xC0\xC3",
        NULL
      },
    }
  },{
    WIN10TH2,4,
    {
      {//sub of KiFilterFiberContext + XX -> call
        17,3,0,0,0,
        //"         x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x48\x8B\xC4\x4C\x89\x48\x20\x44\x89\x40\x18\x89\x50\x10\x89\x48\x08",
        (PBYTE)&"\xB0\x01\xC3",
        NULL
      },
      {//MiValidateSectionCreate
        43,41,9,35,93,
        //"         x   x   x   ?   ?   ?   ?   x   x   x   ?   x   x   x   x   ?   ?   ?   ?   x   x   x   x   x   x   x   ?   ?   ?   ?   x   ?   ?   ?   ?   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x8B\x84\x24\xFF\xFF\xFF\xFF\x89\x44\x24\xFF\x4C\x8B\x8C\x24\xFF\xFF\xFF\xFF\x49\x8B\xD4\x48\x8B\x8C\x24\xFF\xFF\xFF\xFF\xE8\xFF\xFF\xFF\xFF\x44\x8B\xF8\x3D\x2D\x01\x00\x00",
        (PBYTE)&"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90",
        (PBYTE)&"\x90\x90\x90\x90\x90\x90\x90\x90\x90"
      },
      {//SeCompareSigningLevelsForAuditableProcess
        18,6,0,0,0,
        //"         x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x8B\xDA\x48\x8B\xF9\x41\x8A\xD1",
        (PBYTE)&"\xB8\x01\x00\x00\x00\xC3",
        NULL
      },
      {//SeValidateImageData
        21,3,0,0,0,
        //"         x   x   x   ?   ?   ?   ?   x   x   x   x   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x4C\x8B\x15\xFF\xFF\xFF\xFF\x4D\x85\xD2\x74\x03\x49\xFF\xE2\xB8\x28\x04\x00\xC0\xC3",
        (PBYTE)&"\x33\xC0\xC3",
        NULL
      },
    }
  },{
    WIN10AU,4,
    {
      {//sub of KiFilterFiberContext + XX -> call
        17,3,0,0,0,
        //"         x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x48\x8B\xC4\x4C\x89\x48\x20\x44\x89\x40\x18\x89\x50\x10\x89\x48\x08",
        (PBYTE)&"\xB0\x01\xC3",
        NULL
      },
      {//MiValidateSectionCreate
        47,43,9,37,106,
        //"         x   x   x   x   ?   ?   ?   ?   x   x   x   x   ?   x   x   x   x   ?   ?   ?   ?   x   x   x   x   x   x   x   ?   ?   ?   ?   x   ?   ?   ?   ?   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x48\x8B\x84\x24\xFF\xFF\xFF\xFF\x48\x89\x44\x24\xFF\x4C\x8B\x8C\x24\xFF\xFF\xFF\xFF\x49\x8B\xD4\x48\x8B\x8C\x24\xFF\xFF\xFF\xFF\xE8\xFF\xFF\xFF\xFF\x4C\x63\xF8\x41\x81\xFF\x2D\x01\x00\x00",
        (PBYTE)&"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90",
        (PBYTE)&"\x90\x90\x90\x90\x90\x90\x90\x90\x90"
      },
      {//SeCompareSigningLevelsForAuditableProcess
        18,6,0,0,0,
        //"         x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x8B\xDA\x48\x8B\xF9\x41\x8A\xD1",
        (PBYTE)&"\xB8\x01\x00\x00\x00\xC3",
        NULL
      },
      {//SeValidateImageData
        21,3,0,0,0,
        //"         x   x   x   ?   ?   ?   ?   x   x   x   x   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x4C\x8B\x15\xFF\xFF\xFF\xFF\x4D\x85\xD2\x74\x03\x49\xFF\xE2\xB8\x28\x04\x00\xC0\xC3",
        (PBYTE)&"\x33\xC0\xC3",
        NULL
      },
    }
  },{
    WIN10CU,4,
    {
      {//sub of KiFilterFiberContext + XX -> call
        17,3,0,0,0,
        //"         x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x48\x8B\xC4\x4C\x89\x48\x20\x44\x89\x40\x18\x89\x50\x10\x89\x48\x08",
        (PBYTE)&"\xB0\x01\xC3",
        NULL
      },
      {//MiValidateSectionCreate
        47,43,9,37,95,
        //"         x   x   x   x   ?   ?   ?   ?   x   x   x   x   ?   x   x   x   x   ?   ?   ?   ?   x   x   ?   x   x   x   x   ?   ?   ?   ?   x   ?   ?   ?   ?   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x48\x8B\x84\x24\xFF\xFF\xFF\xFF\x48\x89\x44\x24\xFF\x4C\x8B\x8C\x24\xFF\xFF\xFF\xFF\x49\x8B\xFF\x48\x8B\x8C\x24\xFF\xFF\xFF\xFF\xE8\xFF\xFF\xFF\xFF\x4C\x63\xF8\x41\x81\xFF\x2D\x01\x00\x00",
        (PBYTE)&"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90",
        (PBYTE)&"\x90\x90\x90\x90\x90\x90\x90\x90\x90"
      },
      {//SeCompareSigningLevelsForAuditableProcess
        18,6,0,0,0,
        //"         x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x8B\xDA\x48\x8B\xF9\x41\x8A\xD1",
        (PBYTE)&"\xB8\x01\x00\x00\x00\xC3",
        NULL
      },
      {//SeValidateImageData
        19,3,0,0,0,
        //"         x   x   x   x   x   x   x   ?   ?   ?   ?   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x48\x83\xEC\x48\x48\x8B\x05\xFF\xFF\xFF\xFF\x4C\x8B\xD1\x48\x85\xC0\x74\x1F",
        (PBYTE)&"\x33\xC0\xC3",
        NULL
      }
    }
  },{
    WIN10FCU,4,
    {
      {//sub of KiFilterFiberContext + XX -> call
        17,3,0,0,0,
        //"         x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x48\x8B\xC4\x4C\x89\x48\x20\x44\x89\x40\x18\x89\x50\x10\x89\x48\x08",
        (PBYTE)&"\xB0\x01\xC3",
        NULL
      },
      {//MiValidateSectionCreate
        47,43,9,37,95,
        //"         x   x   x   x   ?   ?   ?   ?   x   x   x   x   ?   x   x   x   x   ?   ?   ?   ?   x   x   ?   x   x   x   x   ?   ?   ?   ?   x   ?   ?   ?   ?   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x48\x8B\x84\x24\xFF\xFF\xFF\xFF\x48\x89\x44\x24\xFF\x4C\x8B\x8C\x24\xFF\xFF\xFF\xFF\x49\x8B\xFF\x48\x8B\x8C\x24\xFF\xFF\xFF\xFF\xE8\xFF\xFF\xFF\xFF\x4C\x63\xF8\x41\x81\xFF\x2D\x01\x00\x00",
        (PBYTE)&"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90",
        (PBYTE)&"\x90\x90\x90\x90\x90\x90\x90\x90\x90"
      },
      {//SeCompareSigningLevelsForAuditableProcess
        18,6,0,0,0,
        //"         x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x8B\xDA\x48\x8B\xF9\x41\x8A\xD1",
        (PBYTE)&"\xB8\x01\x00\x00\x00\xC3",
        NULL
      },
      {//SeValidateImageData
        19,3,0,0,0,
        //"         x   x   x   x   x   x   x   ?   ?   ?   ?   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x48\x83\xEC\x48\x48\x8B\x05\xFF\xFF\xFF\xFF\x4C\x8B\xD1\x48\x85\xC0\x74\x1F",
        (PBYTE)&"\x33\xC0\xC3",
        NULL
      }
    }
  }
};

PatchTable winloadTable[WVERSIONS]={
  {
    VISTA,1,
    {
      {
        24,4,0,0,0,
        //"         x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x48\x8B\xC4\x53\x55\x57\x41\x54\x41\x55\x48\x83\xEC\x50\x45\x33\xED\x44\x8B\xE1\x4C\x8D\x40\x18",
        (PBYTE)&"\xB0\x01\xC3\x90",
        NULL
      },
    }
  },{
    WIN7,1,
    {
      {
        24,4,0,0,0,
        //"         x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x48\x8B\xC4\x53\x55\x57\x41\x54\x41\x55\x48\x83\xEC\x50\x45\x33\xED\x44\x8B\xE1\x4C\x8D\x40\x18",
        (PBYTE)&"\xB0\x01\xC3\x90",
        NULL
      }
    }
  },{
    WIN8,1,
    {
      {
        29,3,0,0,0,
        //"         x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x48\x89\x7C\x24\x18\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x90",
        (PBYTE)&"\x33\xC0\xC3",
        NULL
      }
    }
  },{
    WIN81,1,
    {
      {
        30,3,0,0,0,
        //"         x   x   x   x   ?   x   x   x   x   ?   x   x   x   x   ?   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x48\x89\x5C\x24\xFF\x4C\x89\x4C\x24\xFF\x48\x89\x4C\x24\xFF\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24",
        (PBYTE)&"\x33\xC0\xC3",
        NULL
      }
    }
  },{
    WIN10,2,
    {
      {
        22,1,0,0,0,
        //"         x   ?   x   x   x   x   x   x   x   x   x   ?   ?   ?   ?   x   x   x   x   x   x   x"
        (PBYTE)&"\x79\xFF\x44\x8B\xC6\x8B\xD0\x49\x8B\xCF\xE8\xFF\xFF\xFF\xFF\x45\x33\xC9\x8B\xD8\x85\xC0",
        (PBYTE)&"\xEB",
        NULL
      },
      {
        26,6,0,0,0,
        //"         x   x   ?   ?   ?   ?   x   x   x   x   x   x   x   ?   ?   ?   ?   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x0F\x88\xFF\xFF\xFF\xFF\x80\x7C\x24\x61\x00\x0F\x84\xFF\xFF\xFF\xFF\x48\x8B\x45\xB0\x48\x85\xC0\x75\x1E",
        (PBYTE)&"\x90\x90\x90\x90\x90\x90",
        NULL
      }
    }
  },{
    WIN10TH2,2,
    {
      {
        22,1,0,0,0,
        //"         x   ?   x   x   x   x   x   x   x   x   x   ?   ?   ?   ?   x   x   x   x   x   x   x"
        (PBYTE)&"\x79\xFF\x44\x8B\xC6\x8B\xD0\x49\x8B\xCF\xE8\xFF\xFF\xFF\xFF\x45\x33\xC9\x8B\xD8\x85\xC0",
        (PBYTE)&"\xEB",
        NULL
      },
      {
        26,6,0,0,0,
        //"         x   x   ?   ?   ?   ?   x   x   x   x   x   x   x   ?   ?   ?   ?   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x0F\x88\xFF\xFF\xFF\xFF\x80\x7C\x24\x61\x00\x0F\x84\xFF\xFF\xFF\xFF\x48\x8B\x45\xB0\x48\x85\xC0\x75\x1E",
        (PBYTE)&"\x90\x90\x90\x90\x90\x90",
        NULL
      }
    }
  },{
    WIN10AU,3,
    {
      {
        22,1,0,0,0,
        //"         x   ?   x   x   x   x   x   x   x   x   x   ?   ?   ?   ?   x   x   x   x   x   x   x"
        (PBYTE)&"\x79\xFF\x44\x8B\xC6\x8B\xD0\x49\x8B\xCF\xE8\xFF\xFF\xFF\xFF\x45\x33\xC9\x8B\xD8\x85\xC0",
        (PBYTE)&"\xEB",
        NULL
      },
      {
        25,2,0,0,0,
        //"         x   x   ?   ?   ?   ?   x   x   x   x   ?   ?   ?   ?   x   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x0F\x85\xFF\xFF\xFF\xFF\x8A\x55\xCC\xBB\xFF\xFF\xFF\xFF\x48\x8B\x45\xF8\x8A\xCA\x48\x89\x44\x24\x70",
        (PBYTE)&"\x90\xE9",
        NULL
      },
      {
        24,1,0,0,0,
        //"         x   ?   x   x   x   ?   ?   ?   ?   x   x   x   x   x   x   ?   ?   ?   ?   x   x   x   x   x"
        (PBYTE)&"\x79\xFF\x48\x8B\x85\xFF\xFF\xFF\xFF\x45\x8B\xC6\x4C\x8B\xBD\xA0\x05\x00\x00\x8B\xD3\x83\x08\x01",
        (PBYTE)&"\xEB",
        NULL
      }
    }
  },{
    WIN10CU,3,
    {
      {
        22,1,0,0,0,
        //"         x   ?   x   x   x   x   x   x   x   x   x   ?   ?   ?   ?   x   x   x   x   x   x   x"
        (PBYTE)&"\x79\xFF\x44\x8B\xC6\x8B\xD0\x49\x8B\xCF\xE8\xFF\xFF\xFF\xFF\x45\x33\xC9\x8B\xD8\x85\xC0",
        (PBYTE)&"\xEB",
        NULL
      },
      {
        27,2,0,0,0,
        //"         x   x   ?   ?   ?   ?   x   x   ?   ?   x   ?   ?   ?   ?   x   x   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x0F\x85\xFF\xFF\xFF\xFF\x8A\x54\xFF\xFF\xBB\xFF\xFF\xFF\xFF\x48\x8B\x45\x18\x8A\xCA\x48\x89\x45\x88\xF6\xD1",
        (PBYTE)&"\x90\xE9",
        NULL
      },
      {
        17,1,0,0,0,
        //"         x   ?   x   x   x   ?   ?   ?   ?   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x79\xFF\x48\x8B\x85\xFF\xFF\xFF\xFF\x45\x8B\xC4\x8B\xD3\x83\x08\x01",
        (PBYTE)&"\xEB",
        NULL
      }
    }
  },{
    WIN10FCU,3,
    {
      {
        22,1,0,0,0,
        //"         x   ?   x   x   x   x   x   x   x   x   x   ?   ?   ?   ?   x   x   x   x   x   x   x"
        (PBYTE)&"\x79\xFF\x44\x8B\xC6\x8B\xD0\x49\x8B\xCF\xE8\xFF\xFF\xFF\xFF\x45\x33\xC9\x8B\xD8\x85\xC0",
        (PBYTE)&"\xEB",
        NULL
      },
      {
        27,2,0,0,0,
        //"         x   x   ?   ?   ?   ?   x   x   ?   ?   x   ?   ?   ?   ?   x   x   x   x   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x0F\x85\xFF\xFF\xFF\xFF\x8A\x54\xFF\xFF\xBB\xFF\xFF\xFF\xFF\x48\x8B\x45\x18\x8A\xCA\x48\x89\x45\x88\xF6\xD1",
        (PBYTE)&"\x90\xE9",
        NULL
      },
      {
        17,1,0,0,0,
        //"         x   ?   x   x   x   ?   ?   ?   ?   x   x   x   x   x   x   x   x"
        (PBYTE)&"\x79\xFF\x48\x8B\x85\xFF\xFF\xFF\xFF\x45\x8B\xC4\x8B\xD3\x83\x08\x01",
        (PBYTE)&"\xEB",
        NULL
      }
    }
  }
};

LPCSTR PatchguardDisabler::origntoskrnl=NTOSKRNL;
LPCSTR PatchguardDisabler::origwinload=WINLOAD;

PatchguardDisabler::PatchguardDisabler(IApp *pApp,LPCSTR _szPathName)
  :ntoskrnl(ntoskrnlTable,WVERSIONS),winload(winloadTable,WVERSIONS){
  checked=FALSE;patched=FALSE;deffiles=FALSE;dwLastError=0;
  bcdedit=&pApp->bcdedit;
  strcpy(szPathName,_szPathName);
  GetSystemDirectory(sysDir,MAX_PATH);
}

BOOL PatchguardDisabler::Init(INT vr,LPCSTR nntoskrnl,LPCSTR nwinload){
  if(bcdedit->Loaded()){
    if(bcdedit->current&&stricmp(nntoskrnl,origntoskrnl)&&stricmp(nwinload,origwinload)){
      PCHAR curLoader=bcdedit->current->path;dwLastError=0;
      if(!stricmp(&curLoader[strlen(curLoader)-4],".efi")){
        sprintf(szWinLoadPath,"%s\\%s.efi",sysDir,origwinload);
        sprintf(szNewWinLoadPath,"%s\\%s.efi",sysDir,nwinload);
      }else{
        sprintf(szWinLoadPath,"%s\\%s.exe",sysDir,origwinload);
        sprintf(szNewWinLoadPath,"%s\\%s.exe",sysDir,nwinload);
      }
      sprintf(szNtOSKrnlPath,"%s\\%s.exe",sysDir,origntoskrnl);
      sprintf(szNewNtOSKrnlPath,"%s\\%s.exe",sysDir,nntoskrnl);
      deffiles=!stricmp(nntoskrnl,NEWNTOSKRNL)&&!stricmp(nwinload,NEWWINLOAD);
      return (winload.Init(vr,szWinLoadPath,szNewWinLoadPath,deffiles)&&
        ntoskrnl.Init(vr,szNtOSKrnlPath,szNewNtOSKrnlPath,deffiles));
    }else{
      dwLastError=206;
    }
  }else{
    dwLastError=205;
  }
  return FALSE;
}

BOOL PatchguardDisabler::IsPatched(){
  INT i;
  if(checked){
    return patched;
  }
  bcdPatched=NULL;bcdOriginal=NULL;
  for(i=0;i<bcdedit->numentries;i++){
    if(!strcmp(bcdedit->Entries[i].device,bcdedit->current->device)&&
      strstr(bcdedit->Entries[i].description,szPathName)){
      bcdPatched=&bcdedit->Entries[i];
    }
  }
  patched=(bcdPatched!=NULL);
  if(patched){
    CHAR origDescription[MAX_PATH];
    strcpy(origDescription,bcdPatched->description);
    PCHAR p=strstr(origDescription,szPathName);
    if(p){
      *(--p)=0;
      for(i=0;i<bcdedit->numentries;i++){
        if(!strcmp(bcdedit->Entries[i].device,bcdPatched->device)&&
          !strcmp(bcdedit->Entries[i].description,origDescription)){
          bcdOriginal=&bcdedit->Entries[i];
        }
      }
    }
  }
  checked=TRUE;
  return patched;
}

BOOL PatchguardDisabler::IsCurrentPatched(){
  return (patched&&bcdPatched==bcdedit->current);
}

BOOL PatchguardDisabler::CanBePatched(){
  RtlWow64EnableFsRedirection(FALSE);
  BOOL bRet=!FileExist(szNewNtOSKrnlPath)&&!FileExist(szNewWinLoadPath);
  RtlWow64EnableFsRedirection(TRUE);
  return (bRet||deffiles);
}

BOOL PatchguardDisabler::Patch(BOOL setDefault){
  if(!patched){
    if(bcdedit->current&&ntoskrnl.Patch()&&winload.Patch()){
      CHAR newName[MAX_PATH];
      sprintf(newName,"%s %s",bcdedit->current->description,szPathName);
      if(bcdedit->Copy(bcdedit->current->id,newName,newName)){
        PCHAR kernel=strrchr(ntoskrnl.szNewPath,'\\')+1;
        bcdedit->Set(newName,"path",winload.szNewPath+2);
        bcdedit->Set(newName,"kernel",kernel);
        bcdedit->Set(newName,"nointegritychecks","ON");
        if(setDefault){
          bcdedit->SetDefault(newName);
        }
        //reload data
        bcdedit->Load();
        checked=FALSE;
        return IsPatched();
      }else{
        dwLastError=202;
      }
    }else{
      if(ntoskrnl.dwLastError){
        dwLastError=100+ntoskrnl.dwLastError;
      }else if(winload.dwLastError){
        dwLastError=110+winload.dwLastError;
      }else {
        dwLastError=201;
      }
    }
  }else{
    dwLastError=200;
  }
  return FALSE;
}

BOOL PatchguardDisabler::Restore(){
  CHAR szPatchedFile[MAX_PATH],*szSysFile;
  if(patched&&bcdedit->bootmgr){
    if(bcdOriginal&&!strcmp(bcdedit->bootmgr->_default,bcdPatched->id)){
      bcdedit->SetDefault(bcdOriginal->id);
    }
    if(bcdedit->Delete(bcdPatched->id)){
      szSysFile=strrchr(bcdPatched->path,'\\')+1;
      if(strnicmp(szSysFile,origwinload,strlen(origwinload))){
        sprintf(szPatchedFile,"%s\\%s",sysDir,szSysFile);
        RtlWow64EnableFsRedirection(FALSE);
        if(FileExist(szPatchedFile)){
          if(!DeleteFile(szPatchedFile)){
            //delay until reboot
            MoveFileEx(szPatchedFile,NULL,MOVEFILE_DELAY_UNTIL_REBOOT);
          }
        }
        RtlWow64EnableFsRedirection(TRUE);
      }
      szSysFile=bcdPatched->kernel;
      if(strnicmp(szSysFile,origntoskrnl,strlen(origntoskrnl))){
        sprintf(szPatchedFile,"%s\\%s",sysDir,szSysFile);
        RtlWow64EnableFsRedirection(FALSE);
        if(FileExist(szPatchedFile)){
          if(!DeleteFile(szPatchedFile)){
            //delay until reboot
            MoveFileEx(szPatchedFile,NULL,MOVEFILE_DELAY_UNTIL_REBOOT);
          }
        }
        RtlWow64EnableFsRedirection(TRUE);
      }
      //reload data
      bcdedit->Load();
      checked=FALSE;
      return !IsPatched();
    }else{
      dwLastError=204;
    }
  }else{
    dwLastError=203;
  }
  return FALSE;
}
