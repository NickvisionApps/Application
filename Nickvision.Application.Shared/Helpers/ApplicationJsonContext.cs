using Nickvision.Desktop.Application;
using System.Text.Json.Serialization;

namespace Nickvision.Application.Shared.Helpers;

[JsonSourceGenerationOptions(DefaultIgnoreCondition = JsonIgnoreCondition.WhenWritingNull, WriteIndented = true)]
[JsonSerializable(typeof(WindowGeometry))]
public partial class ApplicationJsonContext : JsonSerializerContext
{

}
